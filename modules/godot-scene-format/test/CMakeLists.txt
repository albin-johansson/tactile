project(tactile-godot-scene-format-test CXX)

set(TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET tactile-godot-scene-format-test)

file(GLOB_RECURSE TACTILE_GODOT_SCENE_FORMAT_TEST_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/*.cpp"
     "${PROJECT_SOURCE_DIR}/**/*.cpp"
     )

add_executable(${TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET} "${TACTILE_GODOT_SCENE_FORMAT_TEST_FILES}")

tactile_prepare_target(${TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET})

target_link_libraries(${TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET}
                      PRIVATE
                      tactile::godot-scene-format
                      tactile::testutil
                      )

if (WIN32)
  add_custom_command(TARGET ${TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET}> $<TARGET_FILE_DIR:${TACTILE_GODOT_SCENE_FORMAT_TEST_TARGET}>
                     COMMAND_EXPAND_LISTS
                     )
endif ()
