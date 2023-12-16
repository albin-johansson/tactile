project(tactile-tiled-json-format CXX)

set(TACTILE_TILED_JSON_FORMAT_TARGET tactile-tiled-json-format)

file(GLOB_RECURSE TACTILE_TILED_JSON_FORMAT_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_TILED_JSON_FORMAT_TARGET} SHARED "${TACTILE_TILED_JSON_FORMAT_SOURCE_FILES}")
add_library(tactile::tiled-json-format ALIAS ${TACTILE_TILED_JSON_FORMAT_TARGET})

tactile_prepare_target(${TACTILE_TILED_JSON_FORMAT_TARGET})

target_compile_definitions(${TACTILE_TILED_JSON_FORMAT_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_TMJ_PLUGIN
                           )

target_include_directories(${TACTILE_TILED_JSON_FORMAT_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(${TACTILE_TILED_JSON_FORMAT_TARGET}
                      PUBLIC
                      tactile::foundation
                      nlohmann_json::nlohmann_json
                      )

install(TARGETS ${TACTILE_TILED_JSON_FORMAT_TARGET} DESTINATION "${TACTILE_BUILD_DIR}/plugins/formats")

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()