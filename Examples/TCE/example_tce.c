// Direktna rekurzija — tail recursive
int factorial(int n, int acc) {
    if (n == 0) return acc;
    return factorial(n - 1, n * acc);
}

// Fibonacci sa accumulatorom — tail recursive
int fib(int n, int a, int b) {
    if (n == 0) return a;
    return fib(n - 1, b, a + b);
}

// Sumiranje — tail recursive
int sum(int n, int acc) {
    if (n == 0) return acc;
    return sum(n - 1, acc + n);
}

// OVO NIJE tail call — pass ne sme da dira ovo
int bad_factorial(int n) {
    if (n == 0) return 1;
    return n * bad_factorial(n - 1);  // mnozenje posle poziva!
}

// Void funkcija — tail recursive
void countdown(int n) {
    if (n == 0) return;
    countdown(n - 1);
}

// Dva tail call-a u istoj funkciji
int collatz(int n, int steps) {
    if (n == 1) return steps;
    if (n % 2 == 0) return collatz(n / 2, steps + 1);
    return collatz(3 * n + 1, steps + 1);
}
