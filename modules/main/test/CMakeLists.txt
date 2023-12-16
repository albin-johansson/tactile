project(tactile-integration-test CXX)

set(TACTILE_INTEGRATION_TEST_TARGET tactile-integration-test)

file(GLOB_RECURSE TACTILE_INTEGRATION_TEST_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/*.hpp"
     "${PROJECT_SOURCE_DIR}/*.cpp"
     "${PROJECT_SOURCE_DIR}/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/**/*.cpp"
     )

add_executable(${TACTILE_INTEGRATION_TEST_TARGET} "${TACTILE_INTEGRATION_TEST_FILES}")

tactile_prepare_target(${TACTILE_INTEGRATION_TEST_TARGET})

target_include_directories(${TACTILE_INTEGRATION_TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Note, here we explicitly link plugin targets to facilitate debugging.
target_link_libraries(${TACTILE_INTEGRATION_TEST_TARGET}
                      PRIVATE
                      tactile::foundation
                      tactile::core
                      tactile::tactile-yml-format
                      tactile::tiled-json-format
                      tactile::tiled-xml-format
                      GTest::gtest
                      )