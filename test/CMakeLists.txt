cmake_minimum_required(VERSION 3.16)

project(tactile-test CXX)

enable_testing()

file(GLOB_RECURSE TEST_FILES
     CONFIGURE_DEPENDS
     unit-tests/**/*.cpp
     unit-tests/**/*.hpp
     unit-tests/test_main.cpp
     )

add_executable(${TACTILE_TEST} ${TEST_FILES})

tactile_set_compile_options(${TACTILE_TEST})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_TEST}
                         PRIVATE
                         /wd4834  # disable [[nodiscard]] warnings
                         )

elseif (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|GNU")
  target_compile_options(${TACTILE_TEST}
                         PRIVATE
                         -Wno-unused-result
                         )
endif ()

target_include_directories(${TACTILE_TEST} PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(${TACTILE_TEST}
                      PRIVATE
                      ${TACTILE_LIB}
                      SDL2::SDL2main
                      GTest::gtest
                      )

target_precompile_headers(${TACTILE_TEST} REUSE_FROM ${TACTILE_LIB})

add_test(NAME ${TACTILE_TEST} COMMAND ${TACTILE_TEST})

copy_directory_post_build(${TACTILE_TEST}
                          ${CMAKE_CURRENT_SOURCE_DIR}/test-resources
                          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-resources)