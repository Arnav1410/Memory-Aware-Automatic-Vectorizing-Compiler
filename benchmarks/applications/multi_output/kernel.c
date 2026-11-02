int multiOutput(int *a, int *b, int *c, int *d, int *e, int n) {
    int i;
    for (i=0; i<n; i=i+1) {
        c[i]=a[i]+b[i];
        d[i]=a[i]-b[i];
        e[i]=a[i]*b[i];
    }
    return i;
}
