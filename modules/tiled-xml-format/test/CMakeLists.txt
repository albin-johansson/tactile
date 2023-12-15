project(tactile-tiled-xml-format-test CXX)

set(TACTILE_TILED_XML_FORMAT_TEST_TARGET tactile-tiled-xml-format-test)

file(GLOB_RECURSE TACTILE_TILED_XML_FORMAT_TEST_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/*.cpp"
     "${PROJECT_SOURCE_DIR}/**/*.cpp"
     )

add_executable(${TACTILE_TILED_XML_FORMAT_TEST_TARGET} ${TACTILE_TILED_XML_FORMAT_TEST_FILES})

tactile_prepare_target(${TACTILE_TILED_XML_FORMAT_TEST_TARGET})

target_link_libraries(${TACTILE_TILED_XML_FORMAT_TEST_TARGET}
                      PRIVATE
                      tactile::tiled-xml-format
                      GTest::gtest
                      )

if (WIN32)
  add_custom_command(TARGET ${TACTILE_TILED_XML_FORMAT_TEST_TARGET} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TACTILE_TILED_XML_FORMAT_TEST_TARGET}> $<TARGET_FILE_DIR:${TACTILE_TILED_XML_FORMAT_TEST_TARGET}>
                     COMMAND_EXPAND_LISTS
                     )
endif ()