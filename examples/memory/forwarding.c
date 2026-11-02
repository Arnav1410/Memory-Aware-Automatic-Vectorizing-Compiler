int basicForward(int *a,int *b,int *c,int *d,int n) {
    int i; for(i=0;i<n;i=i+1) {a[i]=a[i]+b[i]; c[i]=a[i]*3;} return i;
}
int multipleReads(int *a,int *b,int *c,int *d,int n) {
    int i; for(i=0;i<n;i=i+1) {a[i]=a[i]+b[i]; c[i]=a[i]*2; d[i]=a[i]-b[i];} return i;
}
int multipleStores(int *a,int *b,int *c,int *d,int n) {
    int i; for(i=0;i<n;i=i+1) {a[i]=a[i]+b[i]; a[i]=a[i]*2; c[i]=a[i]+1;} return i;
}
int longChain(int *a,int *b,int *c,int *d,int n) {
    int i; for(i=0;i<n;i=i+1) {a[i]=a[i]+b[i]; c[i]=a[i]*3; a[i]=c[i]-b[i]; d[i]=a[i]+c[i];} return i;
}
int localForward(int *a,int *b,int *c,int *d,int n) {
    int t[1024]; int i; for(i=0;i<n;i=i+1) {t[i]=a[i]+b[i]; c[i]=t[i]*3;} return i;
}
int differentBase(int *a,int *b,int *c,int *d,int n) {
    int i; for(i=0;i<n;i=i+1) {a[i]=b[i]+1; c[i]=d[i]*3;} return i;
}
int fusionConsumer(int *a,int *b,int *c,int *d,int n) {
    int t[1024]; int i;
    for(i=0;i<n;i=i+1)t[i]=a[i]+b[i];
    for(i=0;i<n;i=i+1){c[i]=t[i]*2;d[i]=c[i]+1;}
    return i;
}
