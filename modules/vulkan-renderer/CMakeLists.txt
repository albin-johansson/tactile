project(tactile-vulkan-renderer CXX)

find_package(Vulkan REQUIRED)

set(TACTILE_VULKAN_TARGET tactile-vulkan-renderer)

file(GLOB_RECURSE TACTILE_VULKAN_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_VULKAN_TARGET} SHARED "${TACTILE_VULKAN_SOURCE_FILES}")

tactile_prepare_target(${TACTILE_VULKAN_TARGET})

target_compile_definitions(${TACTILE_VULKAN_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_VULKAN_RENDERER
                           )

target_include_directories(${TACTILE_VULKAN_TARGET}
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

target_link_libraries(${TACTILE_VULKAN_TARGET}
                      PUBLIC
                      tactile::foundation

                      PRIVATE
                      SDL2::SDL2
                      imgui::imgui
                      Vulkan::Vulkan
                      )

install(TARGETS ${TACTILE_VULKAN_TARGET} DESTINATION "${TACTILE_BUILD_DIR}/plugins/renderers")
