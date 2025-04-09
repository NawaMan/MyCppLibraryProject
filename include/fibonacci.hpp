/**
 * @file fibonacci.hpp
 * @brief Fibonacci number calculator
 */

#ifndef MYCPPLIBRARY_FIBONACCI_HPP
#define MYCPPLIBRARY_FIBONACCI_HPP

namespace mycpplibrary {

/**
 * @brief Calculate the Fibonacci number for a given input
 * 
 * This function calculates the nth Fibonacci number where:
 * F(0) = 0
 * F(1) = 1
 * F(n) = F(n-1) + F(n-2) for n > 1
 *
 * @param input The position in the Fibonacci sequence (0-based)
 * @return The Fibonacci number at the specified position
 */
int fibonacci(int input);

} // namespace mycpplibrary

#endif // MYCPPLIBRARY_FIBONACCI_HPP
