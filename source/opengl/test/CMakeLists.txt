project(tactile-opengl-test CXX)

file(GLOB_RECURSE TACTILE_OPENGL_TEST_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.hpp"
     )

add_executable(tactile-opengl-test "${TACTILE_OPENGL_TEST_FILES}")

tactile_prepare_target(tactile-opengl-test)

target_link_libraries(tactile-opengl-test
                      PUBLIC
                      tactile::opengl
                      tactile::core
                      GTest::gtest

                      PRIVATE
                      glad::glad
                      )
