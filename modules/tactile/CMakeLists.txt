project(tactile_module CXX)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_library(tactile STATIC)

target_sources(tactile
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/interface_class.hpp"
               "inc/tactile/special_member_functions.hpp"

               PUBLIC FILE_SET "CXX_MODULES" FILES
               "src/core/core.cppm"
               "src/core/core-containers.cppm"
               "src/core/core-memory.cppm"
               "src/core/core-primitives.cppm"
               "src/core/core-result.cppm"
               "src/core/core-strings.cppm"
               "src/core/core-time.cppm"
               "src/io/io.cppm"
               "src/io/io-base64.cppm"
               "src/io/io-compression.cppm"
               "src/numeric/numeric.cppm"
               "src/numeric/numeric-casts.cppm"
               "src/numeric/numeric-hash.cppm"
               "src/numeric/numeric-random.cppm"
               "src/tactile.cppm"

               PRIVATE
               "src/core/result.cpp"
               "src/io/base64.cpp"
               "src/numeric/random.cpp"
               )

tactile_prepare_target(tactile)

target_include_directories(tactile
                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )

if (TACTILE_BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)

  add_executable(tactile_tests)

  tactile_prepare_target(tactile_tests)

  target_sources(tactile_tests
                 PUBLIC FILE_SET "CXX_MODULES" FILES
                 "tests/core/core.cppm"
                 "tests/core/core-error.cppm"
                 "tests/io/io.cppm"
                 "tests/io/io-base64.cppm"
                 "tests/numeric/numeric.cppm"
                 "tests/numeric/numeric-casts.cppm"
                 "tests/tactile_tests.cppm"
                 )

  target_link_libraries(tactile_tests
                        PRIVATE
                        tactile
                        GTest::gtest
                        GTest::gmock
                        )
endif ()
