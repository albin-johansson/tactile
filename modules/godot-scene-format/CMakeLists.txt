project(tactile-godot-scene-format CXX)

set(TACTILE_GODOT_SCENE_FORMAT_TARGET tactile-godot-scene-format)

file(GLOB_RECURSE TACTILE_GODOT_SCENE_FORMAT_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_GODOT_SCENE_FORMAT_TARGET} SHARED "${TACTILE_GODOT_SCENE_FORMAT_SOURCE_FILES}")
add_library(tactile::godot-scene-format ALIAS ${TACTILE_GODOT_SCENE_FORMAT_TARGET})

tactile_prepare_target(${TACTILE_GODOT_SCENE_FORMAT_TARGET})

target_compile_definitions(${TACTILE_GODOT_SCENE_FORMAT_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_GODOT_SCENE_FORMAT
                           )

target_include_directories(${TACTILE_GODOT_SCENE_FORMAT_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(${TACTILE_GODOT_SCENE_FORMAT_TARGET}
                      PUBLIC
                      tactile::foundation
                      )

install(TARGETS ${TACTILE_GODOT_SCENE_FORMAT_TARGET} DESTINATION "${TACTILE_BUILD_DIR}/plugins/formats")

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()