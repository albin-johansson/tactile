project(tactile-core-test CXX)

set(TACTILE_CORE_TEST_TARGET tactile-core-test)

file(GLOB_RECURSE TACTILE_CORE_TEST_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_CORE_DIR}/test/*.hpp"
     "${TACTILE_CORE_DIR}/test/*.cpp"
     "${TACTILE_CORE_DIR}/test/**/*.hpp"
     "${TACTILE_CORE_DIR}/test/**/*.cpp"
     )

add_executable(${TACTILE_CORE_TEST_TARGET} ${TACTILE_CORE_TEST_FILES})

tactile_prepare_target(${TACTILE_CORE_TEST_TARGET})

target_include_directories(${TACTILE_CORE_TEST_TARGET}
                           PRIVATE
                           "${TACTILE_CORE_DIR}/test"
                           )

target_link_libraries(${TACTILE_CORE_TEST_TARGET}
                      PRIVATE
                      tactile::core
                      GTest::gmock
                      )

if (WIN32)
  add_custom_command(TARGET ${TACTILE_CORE_TEST_TARGET} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TACTILE_CORE_TEST_TARGET}> $<TARGET_FILE_DIR:${TACTILE_CORE_TEST_TARGET}>
                     COMMAND_EXPAND_LISTS
                     )
endif ()