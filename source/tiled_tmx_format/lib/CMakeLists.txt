project(tactile-tiled-tmx-format-lib CXX)

add_library(tactile-tiled-tmx-format SHARED)
add_library(tactile::tiled_tmx_format ALIAS tactile-tiled-tmx-format)

target_sources(tactile-tiled-tmx-format
               PRIVATE
               "src/tactile/tiled_tmx_format/tiled_tmx_format_plugin.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/tiled_tmx_format/api.hpp"
               "inc/tactile/tiled_tmx_format/tiled_tmx_format_plugin.hpp"
               )

tactile_prepare_target(tactile-tiled-tmx-format)

target_compile_definitions(tactile-tiled-tmx-format
                           PRIVATE
                           "TACTILE_BUILDING_TILED_TMX_FORMAT"
                           )

target_link_libraries(tactile-tiled-tmx-format
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      pugixml::pugixml
                      )
