#include "collatz.h"
#include "cache.h" 

// Function to perform the Collatz conjecture and return the number of steps
int collatz_steps(CacheNumber num) {
    int steps = 0;
    while (num != 1) {
        if (num % 2 == 0) {
            num /= 2;
        } else {
            num = 3 * num + 1;
        }
        steps++;
    }
    return steps;
}


