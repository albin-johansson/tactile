project(tactile-godot-tscn-format-lib CXX)

add_library(tactile-godot-tscn-format SHARED)
add_library(tactile::godot_tscn_format ALIAS tactile-godot-tscn-format)

target_sources(tactile-godot-tscn-format
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/godot_tscn_format/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/godot_tscn_format/godot_tscn_format_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/godot_tscn_format/godot_tscn_format_plugin.cpp"
               )

tactile_prepare_target(tactile-godot-tscn-format)

target_include_directories(tactile-godot-tscn-format
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

target_compile_definitions(tactile-godot-tscn-format
                           PRIVATE
                           "TACTILE_BUILDING_GODOT_TSCN_FORMAT"
                           )

target_link_libraries(tactile-godot-tscn-format
                      PUBLIC
                      tactile::base
                      tactile::runtime
                      )
