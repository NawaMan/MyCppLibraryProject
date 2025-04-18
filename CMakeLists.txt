cmake_minimum_required(VERSION 3.10)

# Add custom module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set policies
cmake_policy(SET CMP0048 NEW)  # project() command manages VERSION variables
cmake_policy(SET CMP0042 NEW)  # MACOSX_RPATH is enabled by default

# Read version from version.txt
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)

project(MyCppLibrary VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# Output architecture information
message(STATUS "Building for: ${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific settings
if(WIN32)
    # Enable exports in Windows DLL
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    
    # MinGW settings
    if(MINGW)
        # Link statically against the standard libraries
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    endif()
    
    # MSVC settings
    if(MSVC)
        # Use static runtime for Release builds
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        # Enable maximum warning level
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        # Disable specific warnings
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # class needs to have dll-interface
    endif()
endif()

# Optionally enable testing and coverage
option(BUILD_TESTING   "Build tests"               ON)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# Coverage configuration
if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

# Set Boost-related policies
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()
if(POLICY CMP0111)
    cmake_policy(SET CMP0111 NEW)
endif()

# Set Boost library paths for MinGW (Windows cross-compilation)
if(MINGW)
    set(Boost_LOCALE_LIBRARY /usr/x86_64-w64-mingw32/lib/libboost_locale-mt.a)
    set(Boost_REGEX_LIBRARY  /usr/x86_64-w64-mingw32/lib/libboost_regex-mt.a)
    set(Boost_INCLUDE_DIR    /usr/x86_64-w64-mingw32/include)
    set(Boost_LIBRARY_DIR    /usr/x86_64-w64-mingw32/lib)
endif()

# Find Boost
find_package(Boost REQUIRED COMPONENTS locale regex)

# ICU is optional for Windows builds
if(WIN32 OR MINGW)
    set(USE_ICU OFF)
    message(STATUS "Building for Windows - ICU support disabled")
else()
    set(USE_ICU ON)
    # Find ICU components
    find_package(ICU REQUIRED COMPONENTS uc i18n data)
    
    # Set ICU include and library paths explicitly
    set(ICU_INCLUDE_DIRS "/home/linuxbrew/.linuxbrew/include")
    set(ICU_LIBRARIES "/home/linuxbrew/.linuxbrew/lib/libicui18n.so.77" "/home/linuxbrew/.linuxbrew/lib/libicuuc.so.77" "/home/linuxbrew/.linuxbrew/lib/libicudata.so.77")
endif()

# Set IMPORTED_LOCATION for Boost::locale
if(TARGET Boost::locale)
    get_target_property(BOOST_LOCALE_LIB Boost::locale IMPORTED_LOCATION_RELEASE)
    if(NOT BOOST_LOCALE_LIB)
        set_target_properties(Boost::locale PROPERTIES
            IMPORTED_LOCATION_RELEASE "${Boost_LOCALE_LIBRARY_RELEASE}"
            IMPORTED_LOCATION         "${Boost_LOCALE_LIBRARY}")
    endif()
endif()

# Print compiler information
message(STATUS "CMAKE_CXX_COMPILER_ID:        ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION:   ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_COMPILER full path: ${CMAKE_CXX_COMPILER}")

# Set RPATH for executables
set(CMAKE_SKIP_BUILD_RPATH            FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH    TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/lib/x86_64-linux-gnu;/home/linuxbrew/.linuxbrew/lib")

# Add the library
add_library(mycpplibrary_lib
    src/fibonacci.cpp
    src/string_utils.cpp
)
target_include_directories(mycpplibrary_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(USE_ICU)
    target_include_directories(mycpplibrary_lib PUBLIC ${ICU_INCLUDE_DIRS})
endif()

if(MINGW)
    # Set up imported targets for custom-built Boost libraries
    add_library(boost_locale STATIC IMPORTED)
    set_target_properties(boost_locale PROPERTIES
        IMPORTED_LOCATION /usr/x86_64-w64-mingw32/lib/libboost_locale.a)

    add_library(boost_regex STATIC IMPORTED)
    set_target_properties(boost_regex PROPERTIES
        IMPORTED_LOCATION /usr/x86_64-w64-mingw32/lib/libboost_regex.a)

    # Add include directories for Boost
    target_include_directories(mycpplibrary_lib PUBLIC
        /usr/x86_64-w64-mingw32/include
    )

    # Link with the custom-built Boost libraries
    target_link_libraries(mycpplibrary_lib PUBLIC
        boost_locale
        boost_regex
        -static-libgcc
        -static-libstdc++
    )
else()
    target_link_libraries(mycpplibrary_lib PUBLIC
        Boost::locale
        Boost::regex
    )
    
    if(USE_ICU)
        target_link_libraries(mycpplibrary_lib PUBLIC ${ICU_LIBRARIES})
    endif()
endif()


# Set library output directory to dist
set_target_properties(mycpplibrary_lib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist
)

# Add tests if enabled
if(BUILD_TESTING)
    enable_testing()
    
    # Use pkg-config to find GTest
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTEST REQUIRED gtest)

    # Add tests
    add_executable(mycpplibrary_tests
        tests/fibonacci_test.cpp
        tests/string_utils_test.cpp
    )
    target_include_directories(mycpplibrary_tests PRIVATE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(mycpplibrary_tests PRIVATE
        mycpplibrary_lib
        ${GTEST_LIBRARIES}
        pthread
    )
    
    if(USE_ICU)
        target_link_libraries(mycpplibrary_tests PRIVATE ${ICU_LIBRARIES})
    endif()
    # Use GoogleTest module to discover tests
    include(GoogleTest)
    gtest_discover_tests(mycpplibrary_tests)
endif()

# CPack configuration - set variables before including CPack
    
# Basic package information
set(CPACK_PACKAGE_NAME                "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR              "NawaMan")
set(CPACK_PACKAGE_CONTACT             "nawa@nawaman.net")
set(CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MyCppLibrary - A C++ library")

# Package generators
if(WIN32)
    set(CPACK_GENERATOR        "ZIP")
    set(CPACK_SOURCE_GENERATOR "ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR        "TGZ;productbuild")
    set(CPACK_SOURCE_GENERATOR "TGZ")

    # macOS package settings
    set(CPACK_OSX_PACKAGE_VERSION        "10.15")
    set(CPACK_PRODUCTBUILD_DOMAINS       true)
    set(CPACK_PRODUCTBUILD_IDENTIFIER    "net.nawaman.mycpplibrary")
    set(CPACK_PRODUCTBUILD_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/packaging/macos")
else()
    set(CPACK_GENERATOR        "TGZ;DEB;RPM")
    set(CPACK_SOURCE_GENERATOR "TGZ")

    # Debian package settings
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
    # No external dependencies for static build
    set(CPACK_DEBIAN_PACKAGE_SECTION    "libs")

    # RPM package settings
    set(CPACK_RPM_PACKAGE_LICENSE  "MIT")
    set(CPACK_RPM_PACKAGE_GROUP    "Development/Libraries")
    # No external dependencies for static build
endif()

# Finally include CPack
include(CPack)

# Installation configuration
install(TARGETS mycpplibrary_lib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
