/**
 * @file string_utils_test.cpp
 * @brief Tests for the StringUtils class
 */

#include <gtest/gtest.h>
#include "string_utils.hpp"

using namespace mycpplibrary;

TEST(StringUtilsTest, ToLower) {
    EXPECT_EQ("hello world", StringUtils::toLower("Hello World"));
    EXPECT_EQ("café", StringUtils::toLower("CAFÉ"));
    EXPECT_EQ("123", StringUtils::toLower("123"));
}

TEST(StringUtilsTest, ToUpper) {
    EXPECT_EQ("HELLO WORLD", StringUtils::toUpper("Hello World"));
    EXPECT_EQ("CAFÉ", StringUtils::toUpper("café"));
    EXPECT_EQ("123", StringUtils::toUpper("123"));
}

TEST(StringUtilsTest, ExtractEmails) {
    std::string text = "Contact us at info@example.com or support@example.org for help.";
    auto emails = StringUtils::extractEmails(text);
    
    ASSERT_EQ(2, emails.size());
    EXPECT_EQ("info@example.com", emails[0]);
    EXPECT_EQ("support@example.org", emails[1]);
}

TEST(StringUtilsTest, IsValidEmail) {
    EXPECT_TRUE(StringUtils::isValidEmail("user@example.com"));
    EXPECT_TRUE(StringUtils::isValidEmail("user.name@example.co.uk"));
    EXPECT_FALSE(StringUtils::isValidEmail("not-an-email"));
    EXPECT_FALSE(StringUtils::isValidEmail("@example.com"));
    EXPECT_FALSE(StringUtils::isValidEmail("user@"));
}
