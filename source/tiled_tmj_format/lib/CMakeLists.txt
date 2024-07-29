project(tactile-tiled-tmj-format-lib CXX)

add_library(tactile-tiled-tmj-format SHARED)
add_library(tactile::tiled_tmj_format ALIAS tactile-tiled-tmj-format)

target_sources(tactile-tiled-tmj-format
               PRIVATE
               "src/tactile/tiled_tmj_format/tiled_tmj_format.cpp"
               "src/tactile/tiled_tmj_format/tiled_tmj_format_plugin.cpp"
               "src/tactile/tiled_tmj_format/tiled_tmj_format_save_visitor.cpp"
               "src/tmj_format_attribute_parser.cpp"
               "src/tmj_format_layer_emitter.cpp"
               "src/tmj_format_layer_parser.cpp"
               "src/tmj_format_map_emitter.cpp"
               "src/tmj_format_map_parser.cpp"
               "src/tmj_format_meta_emitter.cpp"
               "src/tmj_format_object_emitter.cpp"
               "src/tmj_format_object_parser.cpp"
               "src/tmj_format_tile_emitter.cpp"
               "src/tmj_format_tileset_emitter.cpp"
               "src/tmj_format_tileset_parser.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/tiled_tmj_format/api.hpp"
               "inc/tactile/tiled_tmj_format/tiled_tmj_format.hpp"
               "inc/tactile/tiled_tmj_format/tiled_tmj_format_plugin.hpp"
               "inc/tactile/tiled_tmj_format/tiled_tmj_format_save_visitor.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_layer_emitter.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_layer_parser.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_map_emitter.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_map_parser.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_meta_emitter.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_object_emitter.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_object_parser.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_tile_emitter.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_tileset_emitter.hpp"
               "inc/tactile/tiled_tmj_format/tmj_format_tileset_parser.hpp"
               )

tactile_prepare_target(tactile-tiled-tmj-format)

target_compile_definitions(tactile-tiled-tmj-format
                           PRIVATE
                           "TACTILE_BUILDING_TILED_TMJ_FORMAT"
                           )

target_include_directories(tactile-tiled-tmj-format
                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )

target_link_libraries(tactile-tiled-tmj-format
                      PUBLIC
                      tactile::base
                      tactile::runtime
                      nlohmann_json::nlohmann_json
                      )
