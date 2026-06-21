//
// Created by andrija on 21. 6. 2026..
//

// Pokrenut mem2reg i adce

int helper(int x) {
    return x * 2;
}

int compute(int a, int b) {
    int dead1 = a + b;
    int dead2 = dead1 * 3;
    int dead3 = dead2 - 7;

    int unused_call = helper(a);

    int live1 = a * b;
    int live2 = live1 + 10;

    if (a > 0) {
        return live2;
    } else {
        int never_used = a - b;
        return 0;
    }

    int after_return = a + 1;
    return after_return;
}

int main() {
    int result = compute(5, 3);
    return result;
}
