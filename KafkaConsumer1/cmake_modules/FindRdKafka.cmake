# Override default CMAKE_FIND_LIBRARY_SUFFIXES
# (Allows optional prioritization of static libraries during resolution)
if (CPPKAFKA_RDKAFKA_STATIC_LIB)
    set(RDKAFKA_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
    set(RDKAFKA_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(RDKAFKA_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
    set(RDKAFKA_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

find_path(RDKAFKA_ROOT_DIR
        NAMES include/librdkafka/rdkafka.h
        )

find_path(RDKAFKA_INCLUDE_DIR
        NAMES librdkafka/rdkafka.h
        HINTS ${RDKAFKA_ROOT_DIR}/include
        )

# Check lib paths
if (CPPKAFKA_CMAKE_VERBOSE)
    get_property(FIND_LIBRARY_32 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS)
    get_property(FIND_LIBRARY_64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
    MESSAGE(STATUS "RDKAFKA search 32-bit library paths: ${FIND_LIBRARY_32}")
    MESSAGE(STATUS "RDKAFKA search 64-bit library paths: ${FIND_LIBRARY_64}")
endif()

find_library(RDKAFKA_LIBRARY
        NAMES ${RDKAFKA_PREFIX}rdkafka${RDKAFKA_SUFFIX} rdkafka
        HINTS ${RDKAFKA_ROOT_DIR}/lib
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RDKAFKA DEFAULT_MSG
        RDKAFKA_LIBRARY
        RDKAFKA_INCLUDE_DIR
        )

set(CONTENTS "#include <librdkafka/rdkafka.h>\n #if RD_KAFKA_VERSION >= ${RDKAFKA_MIN_VERSION}\n int main() { }\n #endif")
set(FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/rdkafka_version_test.c)
file(WRITE ${FILE_NAME} ${CONTENTS})

try_compile(HAVE_VALID_KAFKA_VERSION ${CMAKE_CURRENT_BINARY_DIR}
        SOURCES ${FILE_NAME}
        CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${RDKAFKA_INCLUDE_DIR}")

if (HAVE_VALID_KAFKA_VERSION)
    message(STATUS "Found valid rdkafka version")
    mark_as_advanced(
            RDKAFKA_ROOT_DIR
            RDKAFKA_INCLUDE_DIR
            RDKAFKA_LIBRARY
    )
else()
    message(FATAL_ERROR "Failed to find valid rdkafka version")
endif()