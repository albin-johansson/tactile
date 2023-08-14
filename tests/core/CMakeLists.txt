cmake_minimum_required(VERSION 3.16)

project(tactile-tests-core CXX)

file(GLOB_RECURSE TACTILE_CORE_TEST_FILES
     CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/*.cpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     )

set(TACTILE_CORE_TESTS tactile-core-tests)

add_executable(${TACTILE_CORE_TESTS} ${TACTILE_CORE_TEST_FILES})

tactile_set_properties(${TACTILE_CORE_TESTS})
tactile_set_compiler_warnings(${TACTILE_CORE_TESTS})

target_link_libraries(${TACTILE_CORE_TESTS}
                      PRIVATE
                      ${TACTILE_CORE}
                      GTest::gtest
                      )
