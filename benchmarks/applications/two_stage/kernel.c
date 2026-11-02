int twoStage(int *a, int *b, int *c, int *d, int *e, int n) {
    int temporary[1048576];
    int i;
    for (i=0; i<n; i=i+1) temporary[i]=a[i]+b[i];
    for (i=0; i<n; i=i+1) c[i]=temporary[i]*3;
    return i;
}
