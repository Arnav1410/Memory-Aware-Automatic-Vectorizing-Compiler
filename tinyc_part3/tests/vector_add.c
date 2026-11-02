int vecAdd(int *a, int *b, int *c, int n) {
    int i;
    for (i = 0; i < n; i = i + 1)
        c[i] = a[i] + b[i];
    return 0;
}
