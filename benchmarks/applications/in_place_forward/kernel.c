int inPlaceForward(int *a, int *b, int *c, int *d, int *e, int n) {
    int i;
    for (i=0; i<n; i=i+1) {
        a[i]=a[i]+b[i];
        c[i]=a[i]*3;
        d[i]=a[i]-b[i];
        a[i]=d[i]+2;
    }
    return i;
}
