int main() {
    int x = 5;
    int *p;
    p = &x;
    *p = *p + 1;
    return x;
}
