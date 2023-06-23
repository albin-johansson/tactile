cmake_minimum_required(VERSION 3.16)

project(tactile-test CXX)

enable_testing()

file(GLOB_RECURSE TEST_FILES
     CONFIGURE_DEPENDS
     src/**/*.cpp
     src/**/*.hpp
     src/test_main.cpp
     )

add_executable(${TACTILE_TEST} ${TEST_FILES})
add_dependencies(${TACTILE_TEST} ${TACTILE_LIB})

set_target_properties(${TACTILE_TEST}
                      PROPERTIES
                      CXX_STANDARD 23
                      CXX_EXTENSIONS OFF
                      )

tactile_set_compile_options(${TACTILE_TEST})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_TEST}
                         PRIVATE
                         /wd4834  # disable [[nodiscard]] warnings
                         )

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
  target_compile_options(${TACTILE_TEST}
                         PRIVATE
                         -Wno-unused-result
                         )
endif ()

target_include_directories(${TACTILE_TEST} PRIVATE
                           src
                           ${TACTILE_APP_DIR}
                           )

target_link_libraries(${TACTILE_TEST}
                      PRIVATE
                      ${TACTILE_LIB}
                      SDL2::SDL2main
                      doctest::doctest
                      )

target_compile_definitions(${TACTILE_TEST}
                           PRIVATE
                           DOCTEST_CONFIG_USE_STD_HEADERS
                           DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
                           DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION
                           DOCTEST_CONFIG_SUPER_FAST_ASSERTS
                           )

target_precompile_headers(${TACTILE_TEST} REUSE_FROM ${TACTILE_LIB})

add_test(NAME ${TACTILE_TEST} COMMAND ${TACTILE_TEST})

copy_directory_post_build(${TACTILE_TEST}
                          "${CMAKE_CURRENT_SOURCE_DIR}/resources"
                          "${CMAKE_CURRENT_BINARY_DIR}/resources")

copy_directory_post_build(${TACTILE_TEST}
                          ${TACTILE_ASSETS_DIR}
                          "${CMAKE_CURRENT_BINARY_DIR}/assets")