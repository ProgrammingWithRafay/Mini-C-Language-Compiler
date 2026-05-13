int main() {
    int x = 5;
    int y = 10;
    int unused;
    int z;
    
    // Constant folding triggers here (3 * 2 becomes 6)
    z = 3 * 2;
    z = z + x + y;
    
    // Dead code elimination triggers here (unused_var is never read)
    unused = 99;

    printf(z);
    return 0;
}
