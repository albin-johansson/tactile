project(tactile-godot-tscn-format-lib CXX)

add_library(tactile-godot-tscn-format SHARED)
add_library(tactile::godot_tscn_format ALIAS tactile-godot-tscn-format)

target_sources(tactile-godot-tscn-format
               PRIVATE
               "src/gd3_document_converter.cpp"
               "src/gd3_scene_writer.cpp"
               "src/godot_scene_format.cpp"
               "src/godot_tscn_format_plugin.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/godot_tscn_format/api.hpp"
               "inc/tactile/godot_tscn_format/gd3_document_converter.hpp"
               "inc/tactile/godot_tscn_format/gd3_scene_writer.hpp"
               "inc/tactile/godot_tscn_format/gd3_types.hpp"
               "inc/tactile/godot_tscn_format/godot_scene_format.hpp"
               "inc/tactile/godot_tscn_format/godot_tscn_format_plugin.hpp"
               )

tactile_prepare_target(tactile-godot-tscn-format)

target_compile_definitions(tactile-godot-tscn-format
                           PRIVATE
                           "TACTILE_BUILDING_GODOT_TSCN_FORMAT"
                           )

target_link_libraries(tactile-godot-tscn-format
                      PUBLIC
                      tactile::base
                      tactile::runtime
                      )
