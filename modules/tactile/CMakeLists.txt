project(tactile_module CXX)

find_package(Boost REQUIRED COMPONENTS safe_numerics)
find_package(FastFloat CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_library(tactile STATIC)

target_sources(tactile
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "src/core/core.cppm"
               "src/core/core-containers.cppm"
               "src/core/core-fs.cppm"
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
               "src/numeric/numeric-checked.cppm"
               "src/numeric/numeric-concepts.cppm"
               "src/numeric/numeric-constants.cppm"
               "src/numeric/numeric-hash.cppm"
               "src/numeric/numeric-random.cppm"
               "src/numeric/numeric-vec.cppm"
               "src/tactile.cppm"

               PRIVATE
               "src/core/result.cpp"
               "src/core/strings.cpp"
               "src/io/base64.cpp"
               "src/numeric/random.cpp"
               )

tactile_prepare_target(tactile)

target_link_libraries(tactile
                      PRIVATE
                      FastFloat::fast_float

                      PUBLIC
                      Boost::safe_numerics
                      )

target_include_directories(tactile
                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )

if (TACTILE_BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)

  add_executable(tactile_tests)

  tactile_prepare_target(tactile_tests)

  target_sources(tactile_tests
                 PRIVATE
                 "tests/core/error.cpp"
                 "tests/core/strings.cpp"
                 "tests/io/base64.cpp"
                 "tests/numeric/casts.cpp"
                 "tests/numeric/checked.cpp"
                 "tests/numeric/vec.cpp"
                 "tests/main.cpp"
                 )

  target_link_libraries(tactile_tests
                        PRIVATE
                        tactile
                        GTest::gtest
                        )
endif ()
