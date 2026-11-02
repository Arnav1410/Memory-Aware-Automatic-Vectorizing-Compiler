// Native driver only. Every computational kernel is separately compiled from C/TinyC.
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <dlfcn.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sched.h>
#include <stdexcept>
#include <string>
#include <vector>
using Kernel=int(*)(int32_t*,int32_t*,int32_t*,int32_t*,int32_t*,int);
using Count=uint64_t(*)();
using Reset=void(*)();
volatile uint64_t applicationChecksum;
const std::vector<std::string> modes={"scalar","simd","reuse","forward","fusion","auto","gcc","gcc_scalar"};
struct Method {std::string name;Kernel fn;void* handle;};
struct Buffers {
    std::array<std::vector<int32_t>,5> arrays;
    explicit Buffers(int n){for(auto& a:arrays)a.resize(n+64);}
};
using Layout=std::array<std::pair<int,int>,5>;
std::vector<Layout> layouts={
    {{{0,1},{1,1},{2,1},{3,1},{4,1}}}, // disjoint
    {{{0,1},{1,1},{0,1},{3,1},{4,1}}}, // C == A
    {{{0,1},{0,1},{2,1},{3,1},{4,1}}}, // B == A
    {{{0,1},{1,1},{0,2},{3,1},{4,1}}}, // C == A + 1
    {{{0,2},{1,1},{0,1},{3,1},{4,1}}}, // C == A - 1
    {{{0,1},{1,1},{2,1},{2,1},{2,2}}}  // D == C, E == C + 1
};
Method load(const std::string& dir,const std::string& mode,const std::string& fn){
    void* h=dlopen((dir+"/"+mode+".so").c_str(),RTLD_NOW|RTLD_LOCAL);
    if(!h)throw std::runtime_error(dlerror());
    auto f=reinterpret_cast<Kernel>(dlsym(h,fn.c_str()));
    if(!f)throw std::runtime_error(dlerror());
    return {mode,f,h};
}
void initialize(Buffers& b,unsigned seed){
    std::mt19937 rng(seed);
    for(auto& a:b.arrays){for(auto& v:a)v=int(rng()%201)-100;a[0]=-100;a[1]=0;a[2]=100;}
}
void reset(Buffers& dest,const Buffers& src){
    for(int k=0;k<5;++k)std::copy(src.arrays[k].begin(),src.arrays[k].end(),dest.arrays[k].begin());
}
uint64_t calls(Kernel fn,Buffers& b,const Layout& layout,int n,int reps){
    std::array<int32_t*,5> p;
    for(int k=0;k<5;++k)p[k]=b.arrays[layout[k].first].data()+layout[k].second;
    uint64_t sum=0;
    for(int r=0;r<reps;++r)sum+=fn(p[0],p[1],p[2],p[3],p[4],n);
    return sum;
}
void verify(const Buffers& a,const Buffers& b,uint64_t ret,uint64_t want,const std::string& context){
    if(ret!=want||a.arrays!=b.arrays)throw std::runtime_error("complete-allocation/return mismatch: "+context);
}
bool disjoint(const Layout& l,int n,int used){
    for(int a=0;a<used;++a)for(int b=a+1;b<used;++b)
        if(l[a].first==l[b].first && l[a].second<l[b].second+n && l[b].second<l[a].second+n)return false;
    return true;
}
int main(int argc,char**argv){
 try{
    if(argc<5)throw std::runtime_error("runner check|paths|bench LIBDIR FUNCTION N [SAMPLES PATTERN | VECTOR_LOOPS THRESHOLD]");
    static_assert(sizeof(int)==4,"int32 ABI required");
    std::string action=argv[1],dir=argv[2],fn=argv[3];int n=std::stoi(argv[4]);
    if(n<0||n>1048576)throw std::runtime_error("n outside supported local-array capacity");
    Buffers initial(n),expected(n),actual(n);
    auto reference=load(dir,"gcc",fn);
    if(action=="check"){
        std::vector<Method> methods;for(auto& mode:modes)methods.push_back(load(dir,mode,fn));
        int executions=0,comparisons=0;
        for(unsigned seed:{19u,627u,2026u}){
            initialize(initial,seed);
            for(size_t l=0;l<layouts.size();++l){
                reset(expected,initial);uint64_t want=calls(reference.fn,expected,layouts[l],n,1);++executions;
                for(auto& m:methods){
                    if(m.name=="gcc")continue;
                    reset(actual,initial);uint64_t got=calls(m.fn,actual,layouts[l],n,1);++executions;
                    verify(actual,expected,got,want,m.name+" "+fn+" n="+std::to_string(n)+" seed="+std::to_string(seed)+" layout="+std::to_string(l));++comparisons;
                }
            }
        }
        std::cout<<"executions,comparisons\n"<<executions<<','<<comparisons<<'\n';return 0;
    }
    if(action=="paths"){
        if(argc!=7)throw std::runtime_error("paths requires loop count and threshold");
        int loops=std::stoi(argv[5]),threshold=std::stoi(argv[6]),checks=0;
        int used=fn=="multiOutput"?5:fn=="inPlaceForward"?4:3;
        initialize(initial,627);
        for(bool cpu:{true,false}){
            auto m=load(dir,cpu?"auto_on":"auto_off",fn);
            auto count=reinterpret_cast<Count>(dlsym(m.handle,"getcount"));
            auto zero=reinterpret_cast<Reset>(dlsym(m.handle,"resetcount"));
            if(!count||!zero)throw std::runtime_error("path-counter accessor missing");
            for(auto& l:layouts){
                reset(expected,initial);uint64_t want=calls(reference.fn,expected,l,n,1);
                reset(actual,initial);zero();uint64_t got=calls(m.fn,actual,l,n,1);verify(actual,expected,got,want,fn+" instrumented");
                uint64_t batches=cpu&&n>=threshold&&disjoint(l,n,used)?uint64_t(n/8)*loops:0;
                if(count()!=batches)throw std::runtime_error("unexpected vector path count: "+fn+" n="+std::to_string(n)+" actual="+std::to_string(count())+" want="+std::to_string(batches));
                ++checks;
            }
        }
        std::cout<<"executions,comparisons,path_checks\n"<<checks*2<<','<<checks<<','<<checks<<'\n';return 0;
    }
    if(action!="bench"||argc!=7)throw std::runtime_error("bench requires sample count and alignment");
    int samples=std::stoi(argv[5]);std::string pattern=argv[6];
    if(samples<15||(pattern!="aligned"&&pattern!="unaligned"))throw std::runtime_error("samples >=15 and valid alignment required");
    std::vector<Method> methods;for(auto& mode:modes)methods.push_back(load(dir,mode,fn));
    initialize(initial,2026);Layout layout;
    for(int k=0;k<5;++k){
        int offset=16-(reinterpret_cast<uintptr_t>(actual.arrays[k].data())%64)/4+(pattern=="unaligned"?1:0);
        layout[k]={k,offset};
    }
    reset(expected,initial);uint64_t once=calls(reference.fn,expected,layout,n,1);
    for(auto& m:methods){reset(actual,initial);verify(actual,expected,calls(m.fn,actual,layout,n,1),once,m.name);calls(m.fn,actual,layout,n,5);}
    // Identical work/call counts across methods, including stateful updates.
    int reps=std::max(8,std::min(500000,8000000/std::max(1,n)));
    reset(expected,initial);uint64_t want=calls(reference.fn,expected,layout,n,reps);
    std::vector<int> order(methods.size());std::iota(order.begin(),order.end(),0);
    std::mt19937 orderRng(627);
    std::cout<<"mode,sample,repetitions,ns_per_call,cpu_before,cpu_after,correctness\n"<<std::setprecision(12);
    for(int sample=0;sample<samples;++sample){
        // Random permutation followed by cyclic rotations: balanced positions in each eight rounds.
        if(sample%int(methods.size())==0)std::shuffle(order.begin(),order.end(),orderRng);
        else std::rotate(order.begin(),order.begin()+1,order.end());
        for(int index:order){
            auto& m=methods[index];reset(actual,initial);int cpuBefore=sched_getcpu();
            auto start=std::chrono::steady_clock::now();uint64_t sum=calls(m.fn,actual,layout,n,reps);
            auto end=std::chrono::steady_clock::now();int cpuAfter=sched_getcpu();applicationChecksum=sum;
            verify(actual,expected,sum,want,m.name+" repeated");
            std::cout<<m.name<<','<<sample<<','<<reps<<','<<std::chrono::duration<double,std::nano>(end-start).count()/reps<<','<<cpuBefore<<','<<cpuAfter<<",PASS\n";
        }
    }
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
