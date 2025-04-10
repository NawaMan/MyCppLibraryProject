/**
 * @file string_utils.cpp
 * @brief Implementation of string utilities using Boost libraries
 */

#include "string_utils.hpp"
#include <boost/locale/conversion.hpp>
#include <boost/locale/generator.hpp>

namespace mycpplibrary {

std::string StringUtils::toLower(const std::string& input) {
    // Create a locale with the default settings
    boost::locale::generator gen;
    std::locale loc = gen("");
    
    // Convert the string to lowercase using Boost locale
    return boost::locale::to_lower(input, loc);
}

std::string StringUtils::toUpper(const std::string& input) {
    // Create a locale with the default settings
    boost::locale::generator gen;
    std::locale loc = gen("");
    
    // Convert the string to uppercase using Boost locale
    return boost::locale::to_upper(input, loc);
}

std::vector<std::string> StringUtils::extractEmails(const std::string& text) {
    std::vector<std::string> result;
    
    // Regular expression for matching email addresses
    // This is a simplified pattern for demonstration purposes
    boost::regex email_regex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    
    // Find all matches in the text
    boost::sregex_iterator it(text.begin(), text.end(), email_regex);
    boost::sregex_iterator end;
    
    // Add each match to the result vector
    for (; it != end; ++it) {
        result.push_back(it->str());
    }
    
    return result;
}

bool StringUtils::isValidEmail(const std::string& email) {
    // More comprehensive email validation regex
    boost::regex email_regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    
    // Return true if the email matches the regex pattern
    return boost::regex_match(email, email_regex);
}

} // namespace mycpplibrary
