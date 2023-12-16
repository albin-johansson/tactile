project(tactile-testutil CXX)

set(TACTILE_TESTUTIL_TARGET tactile-testutil)

file(GLOB_RECURSE TACTILE_TESTUTIL_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/*.hpp"
     "${PROJECT_SOURCE_DIR}/*.cpp"
     "${PROJECT_SOURCE_DIR}/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/**/*.cpp"
     )

add_library(${TACTILE_TESTUTIL_TARGET} STATIC "${TACTILE_TESTUTIL_SOURCE_FILES}")
add_library(tactile::testutil ALIAS ${TACTILE_TESTUTIL_TARGET})

tactile_prepare_target(${TACTILE_TESTUTIL_TARGET})

target_include_directories(${TACTILE_TESTUTIL_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(${TACTILE_TESTUTIL_TARGET}
                      PUBLIC
                      tactile::foundation
                      GTest::gtest
                      )