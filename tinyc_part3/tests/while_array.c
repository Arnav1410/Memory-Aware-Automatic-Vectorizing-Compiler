int main() {
    int a[16];
    int i;
    i = 0;
    while (i < 16) {
        a[i] = i * 2;
        i = i + 1;
    }
    return a[15];
}
