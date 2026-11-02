// Stateful kernels: every method receives the SAME initial buffers and call count.
// Reset and repeated-GCC oracle evaluation are outside every timed interval.
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <dlfcn.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
using Kernel=int(*)(int32_t*,int32_t*,int32_t*,int32_t*,int);
volatile uint64_t forwardingChecksum;
struct Method {std::string name;Kernel fn;};
struct Buffers {
    std::vector<std::vector<int32_t>> data;
    explicit Buffers(int n):data(4,std::vector<int32_t>(n+40)){}
};
int main(int argc,char**argv) {
 try {
    if(argc!=6)throw std::runtime_error("forward_runner LIBDIR FUNCTION N SAMPLES aligned|unaligned");
    std::string dir=argv[1],name=argv[2],pattern=argv[5];int n=std::stoi(argv[3]),samples=std::stoi(argv[4]);
    if(n<0||samples<3||(pattern!="aligned"&&pattern!="unaligned"))throw std::runtime_error("invalid arguments");
    std::vector<Method> methods;
    for(std::string mode:{"scalar","simd","reuse","forward","fusion","auto","gcc"}) {
        void* handle=dlopen((dir+"/"+mode+".so").c_str(),RTLD_NOW|RTLD_LOCAL);
        if(!handle)throw std::runtime_error(dlerror());
        auto fn=reinterpret_cast<Kernel>(dlsym(handle,name.c_str()));
        if(!fn)throw std::runtime_error(dlerror());
        methods.push_back({mode,fn});
    }
    // Fixed offsets per allocation; all four full allocations including guards verified.
    Buffers actual(n),expected(n),initial(n);
    std::vector<int> offsets(4);
    std::mt19937 rng(20261101);
    for(int k=0;k<4;++k) {
        offsets[k]=16-(reinterpret_cast<uintptr_t>(actual.data[k].data())%64)/4+(pattern=="unaligned"?1:0);
        for(int32_t& v:initial.data[k])v=int(rng()%101)-50;
    }
    auto reset=[&](Buffers& b){for(int k=0;k<4;++k)std::copy(initial.data[k].begin(),initial.data[k].end(),b.data[k].begin());};
    auto calls=[&](Kernel fn,Buffers& b,int reps) {
        uint64_t sum=0;
        for(int r=0;r<reps;++r)sum+=fn(b.data[0].data()+offsets[0],b.data[1].data()+offsets[1],b.data[2].data()+offsets[2],b.data[3].data()+offsets[3],n);
        return sum;
    };
    auto verify=[&](uint64_t got,uint64_t want,const std::string& mode){
        if(got!=want||actual.data!=expected.data)throw std::runtime_error("full-allocation/return check failed: "+name+" "+mode);
    };
    // Verify one invocation before timing any implementation.
    reset(expected);uint64_t once=calls(methods.back().fn,expected,1);
    for(auto& m:methods){reset(actual);verify(calls(m.fn,actual,1),once,m.name);calls(m.fn,actual,5);}
    int reps=std::max(4,std::min(500000,4000000/std::max(1,n)));
    reset(expected);uint64_t wanted=calls(methods.back().fn,expected,reps);
    std::vector<int> order(methods.size());std::iota(order.begin(),order.end(),0);
    std::cout<<"workload,n,mode,sample,repetitions,ns_per_call\n"<<std::setprecision(12);
    for(int sample=0;sample<samples;++sample) {
        std::shuffle(order.begin(),order.end(),rng);
        for(int index:order) {
            auto& m=methods[index];reset(actual);
            auto start=std::chrono::steady_clock::now();
            uint64_t sum=calls(m.fn,actual,reps);
            auto end=std::chrono::steady_clock::now();forwardingChecksum=sum;
            verify(sum,wanted,m.name);
            std::cout<<name<<','<<n<<','<<m.name<<','<<sample<<','<<reps<<','
                <<std::chrono::duration<double,std::nano>(end-start).count()/reps<<'\n';
        }
    }
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
