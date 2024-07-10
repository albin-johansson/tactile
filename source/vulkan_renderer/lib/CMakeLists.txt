project(tactile-vulkan-renderer-lib CXX)

add_library(tactile-vulkan-renderer SHARED)
add_library(tactile::vulkan_renderer ALIAS tactile-vulkan-renderer)

target_sources(tactile-vulkan-renderer
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/vulkan_renderer/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/vulkan_renderer/vulkan_renderer_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/vulkan_renderer/vulkan_renderer_plugin.cpp"
               )

tactile_prepare_target(tactile-vulkan-renderer)

target_include_directories(tactile-vulkan-renderer
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

target_compile_definitions(tactile-vulkan-renderer
                           PRIVATE
                           "TACTILE_BUILDING_VULKAN_RENDERER"
                           )

target_link_libraries(tactile-vulkan-renderer
                      PUBLIC
                      tactile::base
                      tactile::runtime
                      )
