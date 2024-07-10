project(tactile-tiled-tmx-format-lib CXX)

find_package(pugixml CONFIG REQUIRED)

add_library(tactile-tiled-tmx-format SHARED)
add_library(tactile::tiled_tmx_format ALIAS tactile-tiled-tmx-format)

target_sources(tactile-tiled-tmx-format
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/tiled_tmx_format/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/tiled_tmx_format/tiled_tmx_format_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/tiled_tmx_format/tiled_tmx_format_plugin.cpp"
               )

tactile_prepare_target(tactile-tiled-tmx-format)

target_include_directories(tactile-tiled-tmx-format
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

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
