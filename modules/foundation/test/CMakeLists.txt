project(tactile-foundation-test CXX)

set(TACTILE_FOUNDATION_TEST_TARGET tactile-foundation-test)

file(GLOB_RECURSE TACTILE_FOUNDATION_TEST_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_FOUNDATION_DIR}/test/*.hpp"
     "${TACTILE_FOUNDATION_DIR}/test/*.cpp"
     "${TACTILE_FOUNDATION_DIR}/test/**/*.hpp"
     "${TACTILE_FOUNDATION_DIR}/test/**/*.cpp"
     )

add_executable(${TACTILE_FOUNDATION_TEST_TARGET} ${TACTILE_FOUNDATION_TEST_FILES})

tactile_prepare_target(${TACTILE_FOUNDATION_TEST_TARGET})

target_include_directories(${TACTILE_FOUNDATION_TEST_TARGET}
                           PRIVATE
                           "${TACTILE_CORE_DIR}/test"
                           )

target_link_libraries(${TACTILE_FOUNDATION_TEST_TARGET}
                      PRIVATE
                      tactile::foundation
                      GTest::gmock
                      )

if (WIN32)
  add_custom_command(TARGET ${TACTILE_FOUNDATION_TEST_TARGET} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TACTILE_FOUNDATION_TEST_TARGET}> $<TARGET_FILE_DIR:${TACTILE_FOUNDATION_TEST_TARGET}>
                     COMMAND_EXPAND_LISTS
                     )
endif ()