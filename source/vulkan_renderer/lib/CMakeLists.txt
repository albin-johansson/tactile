project(tactile-vulkan-renderer-lib CXX)

add_library(tactile-vulkan-renderer SHARED)
add_library(tactile::vulkan_renderer ALIAS tactile-vulkan-renderer)

target_sources(tactile-vulkan-renderer
               PRIVATE
               "src/tactile/vulkan_renderer/vulkan_renderer_plugin.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/vulkan_renderer/api.hpp"
               "inc/tactile/vulkan_renderer/vulkan_renderer_plugin.hpp"
               )

tactile_prepare_target(tactile-vulkan-renderer)

target_compile_definitions(tactile-vulkan-renderer
                           PRIVATE
                           "TACTILE_BUILDING_VULKAN_RENDERER"
                           )

target_link_libraries(tactile-vulkan-renderer
                      PUBLIC
                      tactile::base
                      tactile::runtime
                      )
