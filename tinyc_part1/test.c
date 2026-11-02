/* TinyC Part 1 lexical-rule coverage. */
int main() {
    int x = 14;
    float y = 3.25e+2;
    char c = '\n';
    x += 1;
    (void)y;
    (void)c;
    if (x >= 10 && x != 0) return x;
    return 0;
}
