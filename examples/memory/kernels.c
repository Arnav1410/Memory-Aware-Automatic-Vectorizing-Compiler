int vectorAdd(int *a, int *b, int *c, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) c[i] = a[i] + b[i];
    return i;
}
int repeatedInputs(int *a, int *b, int *c, int *d, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        c[i] = a[i] + b[i];
        d[i] = a[i] - b[i];
    }
    return i;
}
int producerConsumer(int *a, int *b, int *c, int n) {
    int t[1024];
    int i;
    for (i = 0; i < n; i = i + 1) t[i] = a[i] + b[i];
    for (i = 0; i < n; i = i + 1) c[i] = t[i] * 2;
    return i;
}
