/**
 * @file string_utils.cpp
 * @brief Implementation of string utilities using Boost libraries
 */

#include "string_utils.hpp"
#include <boost/locale/conversion.hpp>
#include <boost/locale/generator.hpp>

// Check if ICU should be used
#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32) && !defined(__MINGW32__)
#define USE_ICU 1
#include <unicode/uscript.h>
#include <unicode/uchar.h>
#include <unicode/utext.h>
#include <unicode/uloc.h>
#include <unicode/utrans.h>
#include <unicode/ubrk.h>
#include <unicode/ucnv.h>
#endif

#include <stdexcept>
#include <memory>
#include <map>
#include <vector>

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

std::string StringUtils::transliterate(const std::string& text, const std::string& translitId) {
#ifdef USE_ICU
    UErrorCode status = U_ZERO_ERROR;
    
    // Create a UnicodeString from the input text
    icu::UnicodeString utext = icu::UnicodeString::fromUTF8(text);
    
    // Create a transliterator
    std::unique_ptr<icu::Transliterator> trans(
        icu::Transliterator::createInstance(icu::UnicodeString::fromUTF8(translitId), 
                                           UTRANS_FORWARD, status));
    
    if (U_FAILURE(status) || !trans) {
        throw std::runtime_error("Failed to create transliterator: " + 
                               std::string(u_errorName(status)));
    }
    
    // Apply the transliteration
    trans->transliterate(utext);
    
    // Convert back to UTF-8
    std::string result;
    utext.toUTF8String(result);
    
    return result;
#else
    // Fallback implementation for platforms without ICU
    return text + " (transliteration not available)";
#endif
}

int StringUtils::countWords(const std::string& text) {
#ifdef USE_ICU
    UErrorCode status = U_ZERO_ERROR;
    
    // Create a UnicodeString from the input text
    icu::UnicodeString utext = icu::UnicodeString::fromUTF8(text);
    
    // Create a word break iterator
    UBreakIterator* ubrk = nullptr;
    ubrk = ubrk_open(UBRK_WORD, uloc_getDefault(), nullptr, 0, &status);
    
    if (U_FAILURE(status) || !ubrk) {
        throw std::runtime_error("Failed to create word break iterator: " + 
                               std::string(u_errorName(status)));
    }
    
    // Create UText from UnicodeString
    UErrorCode textStatus = U_ZERO_ERROR;
    UText* ut = nullptr;
    ut = utext_openUnicodeString(ut, &utext, &textStatus);
    if (U_FAILURE(textStatus)) {
        ubrk_close(ubrk);
        throw std::runtime_error("Failed to create UText: " + 
                               std::string(u_errorName(textStatus)));
    }
    
    // Set the text to analyze
    ubrk_setUText(ubrk, ut, &status);
    
    // Count words (skip non-word characters)
    int wordCount = 0;
    int start = ubrk_first(ubrk);
    
    for (int end = ubrk_next(ubrk); end != UBRK_DONE; 
         start = end, end = ubrk_next(ubrk)) {
        
        // Extract the word
        icu::UnicodeString word = icu::UnicodeString(utext, start, end - start);
        
        // Check if it's a word (contains at least one letter or digit)
        bool isWord = false;
        for (int i = 0; i < word.length(); ++i) {
            if (u_isalpha(word.char32At(i)) || u_isdigit(word.char32At(i))) {
                isWord = true;
                break;
            }
        }
        
        if (isWord) {
            ++wordCount;
        }
    }
    
    // Clean up
    utext_close(ut);
    ubrk_close(ubrk);
    
    return wordCount;
#else
    // Simple fallback implementation for platforms without ICU
    int count = 0;
    bool inWord = false;
    
    for (char c : text) {
        bool isWordChar = std::isalpha(c) || std::isdigit(c);
        
        if (isWordChar && !inWord) {
            // Start of a new word
            inWord = true;
            count++;
        } else if (!isWordChar) {
            // End of a word
            inWord = false;
        }
    }
    
    return count;
#endif
}

std::string StringUtils::detectScript(const std::string& text) {
#ifdef USE_ICU
    UErrorCode status = U_ZERO_ERROR;
    
    // Create a UnicodeString from the input text
    icu::UnicodeString utext = icu::UnicodeString::fromUTF8(text);
    
    // Count occurrences of each script
    std::map<UScriptCode, int> scriptCounts;
    
    for (int32_t i = 0; i < utext.length(); ++i) {
        UChar32 c = utext.char32At(i);
        UScriptCode script = uscript_getScript(c, &status);
        
        if (U_FAILURE(status)) {
            throw std::runtime_error("Failed to get script: " + 
                                   std::string(u_errorName(status)));
        }
        
        // Only count letters and numbers
        if (u_isalnum(c) && script != USCRIPT_COMMON && script != USCRIPT_INHERITED) {
            scriptCounts[script]++;
        }
    }
    
    // Find the dominant script
    UScriptCode dominantScript = USCRIPT_COMMON;
    int maxCount = 0;
    
    for (const auto& pair : scriptCounts) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            dominantScript = pair.first;
        }
    }
    
    // Convert script code to name
    const char* scriptName = uscript_getName(dominantScript);
    return scriptName ? scriptName : "Unknown";
#else
    // Simple fallback implementation for platforms without ICU
    // Just check if it's mostly ASCII or not
    int asciiCount = 0;
    int nonAsciiCount = 0;
    
    for (unsigned char c : text) {
        if (c < 128) {
            asciiCount++;
        } else {
            nonAsciiCount++;
        }
    }
    
    return (asciiCount >= nonAsciiCount) ? "Latin" : "Unknown";
#endif
}

std::string StringUtils::convertEncoding(const std::string& text, 
                                       const std::string& fromEncoding, 
                                       const std::string& toEncoding) {
#ifdef USE_ICU
    UErrorCode status = U_ZERO_ERROR;
    
    // Create converters
    std::unique_ptr<UConverter, void(*)(UConverter*)> fromConverter(
        ucnv_open(fromEncoding.c_str(), &status),
        [](UConverter* conv) { if (conv) ucnv_close(conv); }
    );
    
    if (U_FAILURE(status) || !fromConverter) {
        throw std::runtime_error("Failed to create source converter: " + 
                               std::string(u_errorName(status)));
    }
    
    status = U_ZERO_ERROR;
    std::unique_ptr<UConverter, void(*)(UConverter*)> toConverter(
        ucnv_open(toEncoding.c_str(), &status),
        [](UConverter* conv) { if (conv) ucnv_close(conv); }
    );
    
    if (U_FAILURE(status) || !toConverter) {
        throw std::runtime_error("Failed to create target converter: " + 
                               std::string(u_errorName(status)));
    }
    
    // Convert to Unicode first
    icu::UnicodeString utext(
        text.c_str(), text.length(),
        fromConverter.get(), status
    );
    
    if (U_FAILURE(status)) {
        throw std::runtime_error("Failed to convert to Unicode: " + 
                               std::string(u_errorName(status)));
    }
    
    // Estimate the buffer size needed
    int32_t targetSize = utext.length() * 4; // Conservative estimate
    std::vector<char> buffer(targetSize);
    
    // Convert from Unicode to target encoding
    status = U_ZERO_ERROR;
    int32_t targetLength = ucnv_fromUChars(
        toConverter.get(),
        buffer.data(), buffer.size(),
        utext.getBuffer(), utext.length(),
        &status
    );
    
    if (status == U_BUFFER_OVERFLOW_ERROR) {
        // Resize buffer and try again
        buffer.resize(targetLength + 1);
        status = U_ZERO_ERROR;
        targetLength = ucnv_fromUChars(
            toConverter.get(),
            buffer.data(), buffer.size(),
            utext.getBuffer(), utext.length(),
            &status
        );
    }
    
    if (U_FAILURE(status)) {
        throw std::runtime_error("Failed to convert from Unicode: " + 
                               std::string(u_errorName(status)));
    }
    
    return std::string(buffer.data(), targetLength);
#else
    // Simple fallback implementation for platforms without ICU
    // Just return the original text for Windows builds
    if (fromEncoding == toEncoding) {
        return text;
    } else {
        return text + " (encoding conversion not available)";
    }
#endif
}

} // namespace mycpplibrary
