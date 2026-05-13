int double_val(int n) {
    return n * 2;
}

int main() {
    int x = 5;
    int y = 10;
    int res;
    
    res = double_val(x) + double_val(y);
    printf(res);
    
    return 0;
}
