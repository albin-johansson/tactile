project(tactile-tiled-tmj-format-lib CXX)

add_library(tactile-tiled-tmj-format SHARED)
add_library(tactile::tiled_tmj_format ALIAS tactile-tiled-tmj-format)

target_sources(tactile-tiled-tmj-format
               PRIVATE
               "src/tactile/tiled_tmj_format/tiled_tmj_format_plugin.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/tiled_tmj_format/api.hpp"
               "inc/tactile/tiled_tmj_format/tiled_tmj_format_plugin.hpp"
               )

tactile_prepare_target(tactile-tiled-tmj-format)

target_compile_definitions(tactile-tiled-tmj-format
                           PRIVATE
                           "TACTILE_BUILDING_TILED_TMJ_FORMAT"
                           )

target_link_libraries(tactile-tiled-tmj-format
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      nlohmann_json::nlohmann_json
                      )
