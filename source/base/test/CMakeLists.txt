project(tactile-base-test CXX)

file(GLOB_RECURSE TACTILE_BASE_TEST_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     )

add_executable(tactile-base-test "${TACTILE_BASE_TEST_FILES}")

tactile_prepare_target(tactile-base-test)

target_link_libraries(tactile-base-test
                      PRIVATE
                      tactile::base
                      GTest::gtest
                      )
