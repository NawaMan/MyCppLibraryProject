/**
 * @file fibonacci.cpp
 * @brief Implementation of the Fibonacci function
 */

#include "fibonacci.hpp"

namespace mycpplibrary {

int fibonacci(int input) {
    if (input <= 0) {
        return 0;
    } else if (input == 1) {
        return 1;
    }
    
    int prev = 0;
    int curr = 1;
    int result = 0;
    
    for (int i = 2; i <= input; ++i) {
        result = prev + curr;
        prev = curr;
        curr = result;
    }
    
    return result;
}

} // namespace mycpplibrary
