int add(int a, int b) {
    int x;
    x = a + b * 2;
    if (x > 10)
        return x;
    else
        return x + 1;
}

int main() {
    int i;
    for (i = 0; i < 5; i = i + 1)
        add(i, 3);
    return 0;
}
