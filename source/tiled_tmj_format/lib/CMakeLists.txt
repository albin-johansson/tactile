project(tactile-tiled-tmj-format-lib CXX)

find_package(nlohmann_json CONFIG REQUIRED)

add_library(tactile-tiled-tmj-format SHARED)
add_library(tactile::tiled_tmj_format ALIAS tactile-tiled-tmj-format)

target_sources(tactile-tiled-tmj-format
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/tiled_tmj_format/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/tiled_tmj_format/tiled_tmj_format_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/tiled_tmj_format/tiled_tmj_format_plugin.cpp"
               )

tactile_prepare_target(tactile-tiled-tmj-format)

target_include_directories(tactile-tiled-tmj-format
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

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
