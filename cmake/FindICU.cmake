# FindICU.cmake
#
# Find the International Components for Unicode (ICU) libraries and programs.
#
# This module supports multiple components:
# - uc:     ICU Unicode library
# - i18n:   ICU Internationalization library
# - data:   ICU Data library
# - io:     ICU IO library
# - le:     ICU Layout Engine
# - lx:     ICU Layout Extensions
#
# The following variables will be defined:
#
# ICU_FOUND                   - True if all requested components were found
# ICU_INCLUDE_DIRS            - ICU include directories
# ICU_LIBRARIES               - ICU libraries
# ICU_<component>_FOUND       - True if <component> was found
# ICU_<component>_LIBRARIES   - ICU <component> libraries
# ICU_VERSION                 - ICU version

# Find the ICU include directory
find_path(ICU_INCLUDE_DIR
    NAMES unicode/utypes.h
    DOC "ICU include directory"
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
)

# Get ICU version from unicode/uversion.h
if(ICU_INCLUDE_DIR AND EXISTS "${ICU_INCLUDE_DIR}/unicode/uversion.h")
    file(STRINGS "${ICU_INCLUDE_DIR}/unicode/uversion.h" icu_version_str
        REGEX "^#define U_ICU_VERSION_MAJOR_NUM[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define U_ICU_VERSION_MAJOR_NUM[ \t]+([0-9]+)$" "\\1"
        icu_version_major "${icu_version_str}")

    file(STRINGS "${ICU_INCLUDE_DIR}/unicode/uversion.h" icu_version_str
        REGEX "^#define U_ICU_VERSION_MINOR_NUM[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define U_ICU_VERSION_MINOR_NUM[ \t]+([0-9]+)$" "\\1"
        icu_version_minor "${icu_version_str}")

    file(STRINGS "${ICU_INCLUDE_DIR}/unicode/uversion.h" icu_version_str
        REGEX "^#define U_ICU_VERSION_PATCHLEVEL_NUM[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define U_ICU_VERSION_PATCHLEVEL_NUM[ \t]+([0-9]+)$" "\\1"
        icu_version_patch "${icu_version_str}")

    set(ICU_VERSION "${icu_version_major}.${icu_version_minor}.${icu_version_patch}")
endif()

# Define components
set(ICU_KNOWN_COMPONENTS uc i18n data io le lx)

# Process components
set(ICU_LIBRARIES)
set(ICU_REQUIRED_VARS ICU_INCLUDE_DIR)

foreach(component ${ICU_FIND_COMPONENTS})
    string(TOUPPER "${component}" component_upcase)
    
    # Find the library based on component name
    if(component STREQUAL "uc")
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES icuuc libicuuc
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    elseif(component STREQUAL "i18n")
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES icui18n libicui18n icuin libicuin
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    elseif(component STREQUAL "data")
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES icudata libicudata
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    elseif(component STREQUAL "io")
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES icuio libicuio
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    elseif(component STREQUAL "le")
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES icule libicule
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    elseif(component STREQUAL "lx")
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES iculx libiculx
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    else()
        find_library(ICU_${component_upcase}_LIBRARY
            NAMES icu${component} libicu${component}
            PATHS
                /usr/lib
                /usr/lib64
                /usr/local/lib
                /usr/local/lib64
                /opt/local/lib
                /home/linuxbrew/.linuxbrew/lib
        )
    endif()
    
    if(ICU_${component_upcase}_LIBRARY)
        set(ICU_${component}_FOUND TRUE)
        list(APPEND ICU_LIBRARIES ${ICU_${component_upcase}_LIBRARY})
        list(APPEND ICU_REQUIRED_VARS ICU_${component_upcase}_LIBRARY)
    else()
        set(ICU_${component}_FOUND FALSE)
    endif()
endforeach()

# Set include directories
set(ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIR})

# Handle standard args
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ICU
    REQUIRED_VARS ${ICU_REQUIRED_VARS}
    VERSION_VAR ICU_VERSION
    HANDLE_COMPONENTS
)

# Mark variables as advanced
mark_as_advanced(
    ICU_INCLUDE_DIR
    ${ICU_LIBRARIES}
)
