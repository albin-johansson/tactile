project(tactile-core-test CXX)

file(GLOB_RECURSE TACTILE_CORE_TEST_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     )

add_executable(tactile-core-test "${TACTILE_CORE_TEST_FILES}")

tactile_prepare_target(tactile-core-test)

target_link_libraries(tactile-core-test
                      PUBLIC
                      tactile::core
                      GTest::gtest
                      )
