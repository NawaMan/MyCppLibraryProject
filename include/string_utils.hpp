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

// Check if ICU should be used
#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32) && !defined(__MINGW32__)
#define USE_ICU 1
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/ucnv.h>
#include <unicode/ubrk.h>
#endif

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
    
    /**
     * @brief Transliterate text from one script to another using ICU4C
     * @param text The text to transliterate
     * @param translitId The transliterator ID (e.g., "Latin-Cyrillic")
     * @return The transliterated text
     */
    static std::string transliterate(const std::string& text, const std::string& translitId);
    
    /**
     * @brief Count words in a text using ICU4C's word boundary analysis
     * @param text The text to analyze
     * @return The number of words in the text
     */
    static int countWords(const std::string& text);
    
    /**
     * @brief Detect the script of a text using ICU4C
     * @param text The text to analyze
     * @return The dominant script name (e.g., "Latin", "Cyrillic", etc.)
     */
    static std::string detectScript(const std::string& text);
    
    /**
     * @brief Convert text between different encodings using ICU4C
     * @param text The text to convert
     * @param fromEncoding Source encoding (e.g., "UTF-8", "ISO-8859-1")
     * @param toEncoding Target encoding (e.g., "UTF-8", "UTF-16")
     * @return The converted text
     */
    static std::string convertEncoding(const std::string& text, 
                                      const std::string& fromEncoding, 
                                      const std::string& toEncoding);
};

} // namespace mycpplibrary

#endif // MYCPPLIBRARY_STRING_UTILS_HPP
