#include "../../backend/memory/compiler.hpp"
#include <cassert>
#include <iostream>
#include "forward_unit.hpp"
using namespace memory;
int main(int argc, char** argv) {
    forwardingUnits();
    assert(argc == 2);
    Program p = readProgram(argv[1]);
    Options o;
    assert(p.functions.size() == 3);
    auto add = analyze(p.functions[0],o);
    assert(add.size() == 1 && add[0].legal && add[0].guards);
    assert(add[0].reads.size() == 2 && add[0].writes.size() == 1);
    auto repeated = analyze(p.functions[1],o);
    auto before = statistics(repeated[0].nodes);
    auto after = statistics(reuseLoads(repeated[0].nodes));
    assert(before.loads == 4 && after.loads == 2 && after.stores == 2);
    auto pair = analyze(p.functions[2],o);
    std::vector<Node> joined; std::string reason;
    assert(fuse(p.functions[2],pair[0],pair[1],joined,reason));
    assert(statistics(joined).loads == 2 && statistics(joined).stores == 1);
    auto consumer = pair[1]; consumer.start = 1;
    assert(!fuse(p.functions[2],pair[0],consumer,joined,reason));
    o.alias = "reject";
    assert(!analyze(p.functions[0],o)[0].legal);
    // A write invalidates only the proven same-base cached load.
    std::vector<Node> invalidation = {{"load","a",-1,-1},{"store","a",0,-1},
                                      {"load","a",-1,-1},{"store","c",2,-1}};
    assert(statistics(reuseLoads(invalidation)).loads == 2);
    Loop shortLoop=add[0]; shortLoop.bound="1";
    Candidate scalar; scalar.name="scalar"; scalar.nodes=shortLoop.nodes; scalar.stats=statistics(scalar.nodes);
    Candidate simd=scalar; simd.name="simd";
    assert(estimate(shortLoop,scalar,p.functions[0],o.weights)<estimate(shortLoop,simd,p.functions[0],o.weights));
    shortLoop.bound="1024";
    assert(estimate(shortLoop,simd,p.functions[0],o.weights)<estimate(shortLoop,scalar,p.functions[0],o.weights));
    Loop dynamic=add[0];
    long threshold=profitabilityThreshold(dynamic,simd,p.functions[0],o.weights);
    assert(threshold==8);
    Weights expensive=o.weights; expensive.setup=1000;
    threshold=profitabilityThreshold(dynamic,simd,p.functions[0],expensive);
    assert(threshold>8 && threshold%8==0);
    for(long n=threshold;n<threshold+24;++n) {
        Loop actual=dynamic;actual.bound=std::to_string(actual.start+n);
        assert(estimate(actual,simd,p.functions[0],expensive)<estimate(actual,scalar,p.functions[0],expensive));
    }
    Candidate fused=simd;fused.name="fusion";
    assert(profitabilityThreshold(dynamic,fused,p.functions[0],o.weights)==0);
    std::cout << "analysis, reuse invalidation, fusion legality, statistics and cost unit tests: PASS\n";
}
