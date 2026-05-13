int main() {
    int a = 15;
    int b = 20;
    int c = 25;
    int max;

    if (a > b) {
        if (a > c) {
            max = a;
        } else {
            max = c;
        }
    } else {
        if (b > c) {
            max = b;
        } else {
            max = c;
        }
    }
    
    printf(max);
    return 0;
}
