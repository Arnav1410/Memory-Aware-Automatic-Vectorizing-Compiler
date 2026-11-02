int reuseExample(int *a, int *b, int *c, int *d, int n) {
    int i;
    for (i=0; i<n; i=i+1) {
        c[i]=a[i]+b[i];
        d[i]=a[i]-b[i];
    }
    return i;
}
int fusionExample(int *a, int *b, int *c, int *d, int n) {
    int temporary[1024];
    int i;
    for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
    for (i=0; i<n; i=i+1) c[i]=temporary[i]*2;
    return i;
}
int shortExample(int *a, int *b, int *c, int *d, int n) {
    int i;
    for (i=0; i<1; i=i+1) c[i]=a[i]+b[i];
    return i;
}
int dynamicExample(int *a, int *b, int *c, int *d, int n) {
    int i;
    for (i=0; i<n; i=i+1) c[i]=a[i]+b[i];
    return i;
}
int forwardExample(int *a, int *b, int *c, int *d, int n) {
    int i;
    for(i=0;i<n;i=i+1) {
        a[i]=a[i]+b[i];
        c[i]=a[i]*3;
    }
    return i;
}
