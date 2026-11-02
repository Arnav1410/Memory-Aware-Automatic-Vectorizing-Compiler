#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
extern "C" {
int reuseExample(int*,int*,int*,int*,int);
int fusionExample(int*,int*,int*,int*,int);
int shortExample(int*,int*,int*,int*,int);
int forwardExample(int*,int*,int*,int*,int);
int dynamicExample(int*,int*,int*,int*,int);
}
using Fn=int(*)(int*,int*,int*,int*,int);
volatile uint64_t presentationChecksum;
int main(int argc,char**argv){
 try{
    const std::string mode=argc>1?argv[1]:"auto";
    struct Case{const char*name;Fn fn;int n;};
    for(auto test:std::vector<Case>{{"reuse",reuseExample,1024},{"fusion",fusionExample,1024},
            {"forward",forwardExample,1024},{"short",shortExample,1},{"dynamic",dynamicExample,7},{"dynamic",dynamicExample,17}}){
        std::array<int,1030>a,b,c,d;
        for(int i=0;i<1030;++i){a[i]=i%31-15;b[i]=i%17-8;c[i]=d[i]=123456;}
        const auto initialA=a,initialB=b,initialC=c,initialD=d;
        auto ea=a,eb=b,ec=c,ed=d;
        auto expected=[&](int calls){
            ea=initialA;eb=initialB;ec=initialC;ed=initialD;
            for(int i=0;i<test.n;++i){
                if(std::string(test.name)=="forward") {
                    // Bounds above keep this demonstration's arithmetic in int32 range.
                    ea[i+1]+=calls*eb[i+1];ec[i+1]=ea[i+1]*3;
                } else {
                    ec[i+1]=(ea[i+1]+eb[i+1])*(std::string(test.name)=="fusion"?2:1);
                    if(std::string(test.name)=="reuse")ed[i+1]=ea[i+1]-eb[i+1];
                }
            }
        };
        expected(1);
        auto invoke=[&](){return test.fn(a.data()+1,b.data()+1,c.data()+1,d.data()+1,test.n);};
        if(invoke()!=test.n||a!=ea||b!=eb||c!=ec||d!=ed)throw std::runtime_error("array/reference mismatch");
        for(int w=0;w<5;++w)invoke();
        std::vector<double> times;
        for(int sample=0;sample<9;++sample){
            a=initialA;b=initialB;c=initialC;d=initialD;
            uint64_t sum=0;auto start=std::chrono::steady_clock::now();
            for(int r=0;r<10000;++r)sum+=invoke();
            auto end=std::chrono::steady_clock::now();presentationChecksum=sum;
            expected(10000);
            if(sum!=uint64_t(test.n)*10000||a!=ea||b!=eb||c!=ec||d!=ed)throw std::runtime_error("post-sample mismatch");
            times.push_back(std::chrono::duration<double,std::nano>(end-start).count()/10000);
        }
        if(a!=ea||b!=eb||c!=ec||d!=ed)throw std::runtime_error("post-timing mismatch");
        std::sort(times.begin(),times.end());
        std::cout<<mode<<','<<test.name<<','<<test.n<<','<<std::setprecision(12)<<times[4]<<",PASS\n";
    }
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}
}
