#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

// Interpret one lane using the backend's wrapping int32 arithmetic.
static std::map<std::string,uint32_t> lane(const std::vector<memory::Node>& nodes,
                                         std::map<std::string,uint32_t> mem) {
    std::vector<uint32_t> values(nodes.size());
    for (size_t i=0;i<nodes.size();++i) {
        const auto& n=nodes[i];
        if(n.op=="load") values[i]=mem.at(n.token);
        else if(n.op=="store") mem[n.token]=values.at(n.a);
        else if(n.op=="broadcast") values[i]=uint32_t(std::stoll(n.token));
        else if(n.op=="+") values[i]=values.at(n.a)+values.at(n.b);
        else if(n.op=="-") values[i]=values.at(n.a)-values.at(n.b);
        else if(n.op=="*") values[i]=values.at(n.a)*values.at(n.b);
        else assert(false);
    }
    return mem;
}
static void forwardingUnits() {
    using namespace memory;
    Loop proof; proof.legal=true; proof.guards=true;
    proof.reads={"a","b","c","unused"}; proof.writes={"a","b","c","d"};
    // Read/write chain, two writes of a, a distinct-base read and repeated uses.
    std::vector<Node> nodes={{"load","a"},{"load","b"},{"+","",0,1},
        {"store","a",2},{"load","a"},{"broadcast","3"},{"*","",4,5},
        {"store","c",6},{"load","c"},{"load","b"},{"-","",8,9},
        {"store","a",10},{"load","a"},{"load","c"},{"+","",12,13},
        {"store","d",14},{"load","unused"},{"broadcast","37"}};
    auto reuse=reuseLoads(nodes);
    auto result=forwardStoredValues(proof,reuse);
    assert(result.rejection.empty() && result.eliminated==3);
    assert(statistics(result.nodes).loads==2 && statistics(result.nodes).stores==4);
    assert(statistics(reuse).loads==5); // old pass still invalidates on writes
    std::string reason; assert(validNodes(result.nodes,reason));
    std::vector<std::string> order;
    for(const auto& n:result.nodes) if(n.op=="store") order.push_back(n.token);
    assert((order==std::vector<std::string>{"a","c","a","d"}));
    for(uint32_t a: {0u,1u,0x7fffffffu,0xffffffffu})
        for(uint32_t b: {0u,2u,0x80000000u,0xfffffffdu}) {
            std::map<std::string,uint32_t> initial={{"a",a},{"b",b},{"c",91},{"d",17},{"unused",23}};
            assert(lane(nodes,initial)==lane(result.nodes,initial));
        }
    // Store operand remapping (not the store ID), latest write, and dead pruning.
    std::vector<Node> overwritten={{"broadcast","99"},{"broadcast","2"},{"store","a",1},
        {"load","a"},{"store","c",3},{"broadcast","7"},{"store","a",5},
        {"load","a"},{"store","d",7}};
    auto latest=forwardStoredValues(proof,overwritten);
    assert(latest.eliminated==2 && latest.nodes.size()==6);
    assert(latest.nodes[1].op=="store" && latest.nodes[1].a==0);
    assert(latest.nodes[5].op=="store" && latest.nodes[5].a==3);
    assert(lane(overwritten,{})==lane(latest.nodes,{}));
    // Read-to-read reuse belongs exclusively to the preceding pass.
    std::vector<Node> reads={{"load","a"},{"load","a"},{"+","",0,1},{"store","c",2}};
    assert(forwardStoredValues(proof,reads).eliminated==0);
    assert(statistics(forwardStoredValues(proof,reads).nodes).loads==2);
    // A fresh invocation has no prior iteration's available values.
    assert(forwardStoredValues(proof,{{"load","a"},{"store","c",0}}).eliminated==0);
    for(auto bad: std::vector<std::vector<Node>>{
            {{"call","unknown"}}, {{"load","a",0}}, {{"store","a",4}},
            {{"load","a"},{"store","a",0},{"store","c",1}},
            {{"load","a"},{"+","",0,3}}, {{"load","unproven"}}})
        assert(!forwardStoredValues(proof,bad).rejection.empty());
    proof.legal=false; assert(!forwardStoredValues(proof,reads).rejection.empty()); proof.legal=true;
    // Many stores used much later: forwarding increases lifetime beyond 16 YMMs.
    std::vector<Node> pressure;
    for(int i=0;i<18;++i) {
        std::string base="t"+std::to_string(i); proof.reads.insert(base); proof.writes.insert(base);
        int value=pressure.size(); pressure.push_back({"broadcast",std::to_string(i)});
        pressure.push_back({"store",base,value});
    }
    for(int i=0;i<18;++i) {
        int value=pressure.size(); pressure.push_back({"load","t"+std::to_string(i)});
        pressure.push_back({"store","c",value});
    }
    auto stressed=forwardStoredValues(proof,pressure);
    assert(stressed.eliminated==18 && statistics(pressure).peak==1);
    assert(statistics(stressed.nodes).peak==18 && validNodes(stressed.nodes,reason));
    assert(lane(pressure,{})==lane(stressed.nodes,{}));
    std::cout<<"forwarding: remapping, ordered/latest stores, dead pruning, invalid IR, scoped availability, wrapping semantics and register pressure: PASS\n";
}
