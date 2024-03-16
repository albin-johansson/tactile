project(tactile-test CXX)

find_package(doctest CONFIG REQUIRED)

file(GLOB_RECURSE TEST_FILES
     CONFIGURE_DEPENDS
     src/**/*.cpp
     src/**/*.hpp
     src/test_main.cpp
     )

add_executable(${TACTILE_TEST} ${TEST_FILES})

tactile_prepare_target(${TACTILE_TEST})

set(TACTILE_TEST_EXTRA_COMPILE_OPTIONS)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  list(APPEND
       ${TACTILE_TEST_EXTRA_COMPILE_OPTIONS}
       /wd4834  # disable [[nodiscard]] warnings
       )

else ()
  list(APPEND
       ${TACTILE_TEST_EXTRA_COMPILE_OPTIONS}
       -Wno-unused-result
       )
endif ()

message(DEBUG "TACTILE_TEST_EXTRA_COMPILE_OPTIONS: ${TACTILE_TEST_EXTRA_COMPILE_OPTIONS}")

target_include_directories(${TACTILE_TEST}
                           PRIVATE
                           ${PROJECT_SOURCE_DIR}/src
                           "${TACTILE_ROOT_DIR}/source/app"
                           )

target_link_libraries(${TACTILE_TEST}
                      PRIVATE
                      tactile::lib
                      SDL2::SDL2main
                      doctest::doctest
                      )

target_compile_definitions(${TACTILE_TEST}
                           PRIVATE
                           DOCTEST_CONFIG_USE_STD_HEADERS
                           DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
                           DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION
                           )

tactile_copy_directory_post_build(${TACTILE_TEST}
                                  "${CMAKE_CURRENT_SOURCE_DIR}/resources"
                                  "${CMAKE_CURRENT_BINARY_DIR}/resources")

tactile_copy_directory_post_build(${TACTILE_TEST}
                                  "${TACTILE_RESOURCE_DIR}"
                                  "${CMAKE_CURRENT_BINARY_DIR}/assets")