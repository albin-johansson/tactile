project(tactile-editor-test CXX)

set(TACTILE_EDITOR_TEST_TARGET tactile-editor-test)

file(GLOB_RECURSE TACTILE_EDITOR_TEST_FILES
     CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/*.hpp
     ${PROJECT_SOURCE_DIR}/*.cpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     )

add_executable(${TACTILE_EDITOR_TEST_TARGET} ${TACTILE_EDITOR_TEST_FILES})

tactile_prepare_target(${TACTILE_EDITOR_TEST_TARGET})

target_link_libraries(${TACTILE_EDITOR_TEST_TARGET}
                      PRIVATE
                      ${TACTILE_EDITOR_TARGET}
                      GTest::gtest
                      )