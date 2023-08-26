project(tactile-core-test)

set(TACTILE_CORE_TEST_TARGET tactile-core-test)

file(GLOB_RECURSE TACTILE_CORE_TEST_FILES
     CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/*.hpp
     ${PROJECT_SOURCE_DIR}/*.cpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     )

add_executable(${TACTILE_CORE_TEST_TARGET} ${TACTILE_CORE_TEST_FILES})

tactile_prepare_target(${TACTILE_CORE_TEST_TARGET})

target_link_libraries(${TACTILE_CORE_TEST_TARGET}
                      PRIVATE
                      ${TACTILE_CORE_TARGET}
                      GTest::gtest
                      )