/**
 * @file string_utils.hpp
 * @brief String utilities using Boost libraries
 */

#ifndef MYCPPLIBRARY_STRING_UTILS_HPP
#define MYCPPLIBRARY_STRING_UTILS_HPP

#include <string>
#include <vector>
#include <boost/locale.hpp>
#include <boost/regex.hpp>

namespace mycpplibrary {

/**
 * @brief String utilities class that demonstrates Boost locale and regex usage
 */
class StringUtils {
public:
    /**
     * @brief Convert a string to lowercase using Boost locale
     * @param input The input string
     * @return The lowercase version of the string
     */
    static std::string toLower(const std::string& input);
    
    /**
     * @brief Convert a string to uppercase using Boost locale
     * @param input The input string
     * @return The uppercase version of the string
     */
    static std::string toUpper(const std::string& input);
    
    /**
     * @brief Extract all email addresses from a text using Boost regex
     * @param text The text to search in
     * @return A vector of found email addresses
     */
    static std::vector<std::string> extractEmails(const std::string& text);
    
    /**
     * @brief Validate if a string is a valid email address using Boost regex
     * @param email The email address to validate
     * @return true if the email is valid, false otherwise
     */
    static bool isValidEmail(const std::string& email);
};

} // namespace mycpplibrary

#endif // MYCPPLIBRARY_STRING_UTILS_HPP
