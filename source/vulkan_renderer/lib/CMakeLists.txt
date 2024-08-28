project(tactile-vulkan-renderer-lib CXX)

option(TACTILE_USE_VULKAN_SUBSET "Allow partial Vulkan implementations, such as MoltenVK" OFF)

message("TACTILE_USE_VULKAN_SUBSET: ${TACTILE_USE_VULKAN_SUBSET}")

add_library(tactile-vulkan-renderer SHARED)
add_library(tactile::vulkan_renderer ALIAS tactile-vulkan-renderer)

target_sources(tactile-vulkan-renderer
               PRIVATE
               "src/tactile/vulkan_renderer/vulkan_renderer_plugin.cpp"
               "src/vulkan_allocator.cpp"
               "src/vulkan_fence.cpp"
               "src/vulkan_image.cpp"
               "src/vulkan_image_view.cpp"
               "src/vulkan_instance.cpp"
               "src/vulkan_sampler.cpp"
               "src/vulkan_semaphore.cpp"
               "src/vulkan_shader_module.cpp"
               "src/vulkan_surface.cpp"
               "src/vulkan_util.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/vulkan_renderer/api.hpp"
               "inc/tactile/vulkan_renderer/vulkan_allocator.hpp"
               "inc/tactile/vulkan_renderer/vulkan_fence.hpp"
               "inc/tactile/vulkan_renderer/vulkan_image.hpp"
               "inc/tactile/vulkan_renderer/vulkan_image_view.hpp"
               "inc/tactile/vulkan_renderer/vulkan_instance.hpp"
               "inc/tactile/vulkan_renderer/vulkan_renderer_plugin.hpp"
               "inc/tactile/vulkan_renderer/vulkan_sampler.hpp"
               "inc/tactile/vulkan_renderer/vulkan_semaphore.hpp"
               "inc/tactile/vulkan_renderer/vulkan_shader_module.hpp"
               "inc/tactile/vulkan_renderer/vulkan_surface.hpp"
               "inc/tactile/vulkan_renderer/vulkan_util.hpp"
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
                      Vulkan::Headers
                      volk::volk
                      GPUOpen::VulkanMemoryAllocator
                      SDL2::SDL2

                      PRIVATE
                      magic_enum::magic_enum
                      )

target_compile_definitions(tactile-vulkan-renderer
                           PUBLIC
                           "VK_NO_PROTOTYPES"
                           )
