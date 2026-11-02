#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <dlfcn.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
using Fn=int(*)(int32_t*,int32_t*,int32_t*,int32_t*,int);
volatile uint64_t checksum;
int main(int argc,char**argv){
 try{
    if(argc!=3)throw std::runtime_error("generated LIBDIR SAMPLES");
    std::array<void*,2> handles;
    for(int m=0;m<2;++m){
      std::string path=std::string(argv[1])+(m?"/simd.so":"/scalar.so");
      handles[m]=dlopen(path.c_str(),RTLD_NOW|RTLD_LOCAL);if(!handles[m])throw std::runtime_error(dlerror());
    }
    int samples=std::stoi(argv[2]);std::mt19937 rng(9483);
    std::cout<<"kernel,n,mode,sample,repetitions,ns\n"<<std::setprecision(12);
    for(const std::string name:{"fill","copy","fan","fanwide"})for(int n:{0,1,2,3,4,7,256,4096,65536}){
      std::array<Fn,2> fn;for(int m=0;m<2;++m){fn[m]=reinterpret_cast<Fn>(dlsym(handles[m],name.c_str()));if(!fn[m])throw std::runtime_error(dlerror());}
      std::array<std::vector<int32_t>,4> initial;
      for(auto&v:initial){v.resize(n+2);for(auto&x:v)x=int(rng()%101)-50;}
      auto expected=initial;
      for(int i=1;i<=n;++i){
        if(name=="fill")expected[0][i]=11;
        else {expected[1][i]=initial[0][i];if(name=="fan"||name=="fanwide")expected[2][i]=initial[0][i];if(name=="fanwide")expected[3][i]=initial[0][i];}
      }
      auto data=initial;
      auto invoke=[&](int m){return fn[m](data[0].data()+1,data[1].data()+1,data[2].data()+1,data[3].data()+1,n);};
      auto measure=[&](int m,int reps){
        auto start=std::chrono::steady_clock::now();uint64_t sum=0;
        for(int r=0;r<reps;++r)sum+=invoke(m);
        auto stop=std::chrono::steady_clock::now();checksum=sum;
        return std::chrono::duration<double,std::nano>(stop-start).count()/reps;
      };
      std::array<int,2> reps;
      for(int m=0;m<2;++m){
        data=initial;if(invoke(m)!=n||data!=expected)throw std::runtime_error("calibration correctness "+name);
        for(int w=0;w<5;++w)invoke(m);
        double ns=measure(m,std::max(1,100000/std::max(n,1)));
        reps[m]=std::max(1,std::min(1000000,int(5000000/std::max(1.0,ns))));
      }
      std::array<int,2> order={0,1};
      for(int s=0;s<samples;++s){
        std::shuffle(order.begin(),order.end(),rng);
        for(int m:order){data=initial;double ns=measure(m,reps[m]);
          if(data!=expected)throw std::runtime_error("calibration post-validation "+name);
          std::cout<<name<<','<<n<<','<<(m?"simd":"scalar")<<','<<s<<','<<reps[m]<<','<<ns<<'\n';}
      }
    }
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}
}
