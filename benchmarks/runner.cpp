#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <dlfcn.h>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

using Kernel = int (*)(int32_t*,int32_t*,int32_t*,int32_t*,int);
volatile uint64_t checksum;
struct Method { std::string mode; Kernel fn; int reps=1; };
int main(int argc, char** argv) {
    try {
        if (argc < 5 || argc > 7) throw std::runtime_error("runner LIBDIR FUNCTION N SAMPLES [comma-separated modes] [aligned|unaligned]");
        const std::string dir=argv[1], name=argv[2];
        const int n=std::stoi(argv[3]), samples=std::stoi(argv[4]);
        std::vector<std::string> names={"scalar","simd","reuse","forward","fusion","auto","gcc"};
        if (argc >= 6) {
            names.clear(); std::string list=argv[5]; size_t at=0;
            while (at < list.size()) { size_t end=list.find(',',at); names.push_back(list.substr(at,end-at)); if(end==std::string::npos)break;at=end+1; }
        }
        if (names.empty() || names.back()!="gcc") throw std::runtime_error("last mode must be independent GCC reference");
        const std::string pattern=argc==7?argv[6]:"unaligned";
        if (pattern!="aligned" && pattern!="unaligned") throw std::runtime_error("invalid access alignment");
        std::vector<Method> methods;
        for (const auto& mode:names) {
            void* h=dlopen((dir+"/"+mode+".so").c_str(),RTLD_NOW|RTLD_LOCAL);
            if (!h) throw std::runtime_error(dlerror());
            auto fn=reinterpret_cast<Kernel>(dlsym(h,name.c_str()));
            if (!fn) throw std::runtime_error(dlerror());
            methods.push_back({mode,fn,1});
        }
        // Extra capacity supports the stride-2 held-out evaluation and guard elements.
        std::vector<int32_t> a(2*n+40),b(2*n+40),c(2*n+40,123456),d(2*n+40,123456);
        std::mt19937 rng(20261031);
        for (size_t i=0;i<a.size();++i) {a[i]=int(rng()%101)-50;b[i]=int(rng()%101)-50;}
        auto address=[&](std::vector<int32_t>& x){
            size_t offset=16-(reinterpret_cast<uintptr_t>(x.data())%64)/4;
            return x.data()+offset+(pattern=="unaligned"?1:0);
        };
        int32_t *pa=address(a),*pb=address(b),*pc=address(c),*pd=address(d);
        const auto originalA=a, originalB=b;
        const int expectedRet=methods.back().fn(pa,pb,pc,pd,n);
        const auto expectedC=c,expectedD=d;
        auto verify=[&](Method& m) {
            std::fill(c.begin(),c.end(),123456);std::fill(d.begin(),d.end(),123456);
            int ret=m.fn(pa,pb,pc,pd,n);
            if(ret!=expectedRet||c!=expectedC||d!=expectedD||a!=originalA||b!=originalB)
                throw std::runtime_error("complete-array validation failed: "+m.mode+" "+name);
        };
        auto measure=[&](Method& m,int reps) {
            uint64_t sum=0;
            auto start=std::chrono::steady_clock::now();
            for(int r=0;r<reps;++r) sum+=m.fn(pa,pb,pc,pd,n);
            auto end=std::chrono::steady_clock::now();
            checksum=sum; // External calls cannot be eliminated; consume all return values.
            return std::chrono::duration<double,std::nano>(end-start).count()/reps;
        };
        for(auto& m:methods) {
            verify(m);
            for(int w=0;w<5;++w) m.fn(pa,pb,pc,pd,n);
            double ns=measure(m,std::max(1,100000/std::max(1,n)));
            m.reps=std::max(1,std::min(1000000,int(10000000/std::max(1.0,ns))));
        }
        std::vector<int> order(methods.size());std::iota(order.begin(),order.end(),0);
        std::cout<<"workload,n,mode,sample,repetitions,ns_per_call\n"<<std::setprecision(12);
        for(int sample=0;sample<samples;++sample) {
            std::shuffle(order.begin(),order.end(),rng);
            for(int index:order) {
                Method& m=methods[index];
                double ns=measure(m,m.reps);
                std::cout<<name<<','<<n<<','<<m.mode<<','<<sample<<','<<m.reps<<','<<ns<<'\n';
            }
        }
        for(auto& m:methods) verify(m);
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
