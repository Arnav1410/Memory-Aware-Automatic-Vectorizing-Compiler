// Assembly probes are emitted by calibrate.py, so the optimizer cannot elide work.
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <dlfcn.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
using Probe=void(*)(int32_t*,uint64_t);
using Copy=void(*)(int32_t*,int32_t*,uint64_t);
struct Entry {std::string op;int unroll;Probe fn;};
int main(int argc,char**argv){
 try{
    if(argc!=3)throw std::runtime_error("probe LIBRARY SAMPLES");
    void*h=dlopen(argv[1],RTLD_NOW|RTLD_LOCAL);if(!h)throw std::runtime_error(dlerror());
    int samples=std::stoi(argv[2]);std::mt19937 rng(7331);
    std::vector<int32_t> data(8192,19);std::vector<Entry> entries;
    for(const std::string op:{"control","load","store","add","multiply"})
      for(int unroll:{1,4,8,16,32}){
        if(op=="control"&&unroll!=1)continue;
        auto fn=reinterpret_cast<Probe>(dlsym(h,(op+std::to_string(unroll)).c_str()));
        if(!fn)throw std::runtime_error(dlerror());entries.push_back({op,unroll,fn});
      }
    std::cout<<"kind,unroll,n,sample,repetitions,ns\n"<<std::setprecision(12);
    for(int sample=0;sample<samples;++sample){
      std::shuffle(entries.begin(),entries.end(),rng);
      for(auto e:entries){
        e.fn(data.data(),10000);
        auto start=std::chrono::steady_clock::now();
        e.fn(data.data(),1000000);
        double ns=std::chrono::duration<double,std::nano>(std::chrono::steady_clock::now()-start).count()/1000000;
        std::cout<<e.op<<','<<e.unroll<<",0,"<<sample<<",1000000,"<<ns<<'\n';
      }
    }
    auto copy=reinterpret_cast<Copy>(dlsym(h,"streamcopy"));if(!copy)throw std::runtime_error(dlerror());
    // Different lengths and operations from evaluation kernels; warm steady state.
    for(int n:{1024,4096,16384,65536,262144,2097152}){
      std::vector<int32_t>a(n+2),b(n+2,-99);for(int i=0;i<n;++i)a[i+1]=int(rng());
      int reps=std::max(8,8000000/n);
      for(int w=0;w<5;++w)copy(a.data()+1,b.data()+1,n);
      if(!std::equal(a.begin()+1,a.end()-1,b.begin()+1)||b.front()!=-99||b.back()!=-99)throw std::runtime_error("copy validation");
      for(int s=0;s<samples;++s){
        auto start=std::chrono::steady_clock::now();
        for(int r=0;r<reps;++r)copy(a.data()+1,b.data()+1,n);
        double ns=std::chrono::duration<double,std::nano>(std::chrono::steady_clock::now()-start).count()/reps;
        std::cout<<"stream,0,"<<n<<','<<s<<','<<reps<<','<<ns<<'\n';
      }
      if(!std::equal(a.begin()+1,a.end()-1,b.begin()+1))throw std::runtime_error("copy post-validation");
    }
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}
}
