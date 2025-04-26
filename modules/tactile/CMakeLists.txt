project(tactile_module CXX)

add_library(tactile STATIC)

target_sources(tactile
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "src/core/core.cppm"
               "src/core/core-containers.cppm"
               "src/core/core-memory.cppm"
               "src/core/core-primitives.cppm"
               "src/core/core-result.cppm"
               "src/core/core-strings.cppm"
               "src/core/core-time.cppm"
               "src/numeric/numeric.cppm"
               "src/numeric/numeric-casts.cppm"
               "src/tactile.cppm"

               PRIVATE
               "src/core/result.cpp"
               )

tactile_prepare_target(tactile)

if (TACTILE_BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)

  add_executable(tactile_tests)

  tactile_prepare_target(tactile_tests)

  target_sources(tactile_tests
                 PUBLIC FILE_SET "CXX_MODULES" FILES
                 "tests/core/core.cppm"
                 "tests/core/core-error.cppm"
                 "tests/numeric/numeric.cppm"
                 "tests/numeric/numeric-casts.cppm"
                 "tests/tactile_tests.cppm"
                 )

  target_link_libraries(tactile_tests
                        PRIVATE
                        tactile
                        GTest::gtest
                        )
endif ()
