// Supplemental GCC prologue attribution control; helpers match runner.cpp.
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
    if(argc!=3)throw std::runtime_error("stack_probe LIBDIR N");
    int n=std::stoi(argv[2]);std::string dir=argv[1];
    auto baseline=load(dir,"gcc","twoStage"),control=load(dir,"gcc_noprobe","twoStage");
    Buffers initial(n),expected(n),actual(n);initialize(initial,2026);Layout layout;
    for(int k=0;k<5;++k)layout[k]={k,17-int((reinterpret_cast<uintptr_t>(actual.arrays[k].data())%64)/4)};
    int reps=std::max(1,std::min(2000,1000000/std::max(1,n)));
    reset(expected,initial);uint64_t wanted=calls(baseline.fn,expected,layout,n,reps);
    for(auto m:{baseline,control}){reset(actual,initial);verify(actual,expected,calls(m.fn,actual,layout,n,reps),wanted,m.name);calls(m.fn,actual,layout,n,5);}
    std::cout<<"n,mode,sample,repetitions,ns_per_call,correctness\n"<<std::setprecision(12);
    for(int s=0;s<17;++s){
        std::array<Method,2> order={baseline,control};if(s%2)std::swap(order[0],order[1]);
        for(auto& m:order){reset(actual,initial);auto start=std::chrono::steady_clock::now();uint64_t got=calls(m.fn,actual,layout,n,reps);
            auto end=std::chrono::steady_clock::now();applicationChecksum=got;verify(actual,expected,got,wanted,m.name);
            std::cout<<n<<','<<m.name<<','<<s<<','<<reps<<','<<std::chrono::duration<double,std::nano>(end-start).count()/reps<<",PASS\n";
        }
    }
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
 return 0;
}
