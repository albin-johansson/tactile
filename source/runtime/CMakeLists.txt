project(tactile-runtime CXX)

add_library(tactile-runtime SHARED)
add_library(tactile::runtime ALIAS tactile-runtime)

target_sources(tactile-runtime
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/window.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/window.cpp"
               )

tactile_prepare_target(tactile-runtime)

target_include_directories(tactile-runtime PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_compile_definitions(tactile-runtime PRIVATE "TACTILE_BUILDING_RUNTIME")

target_link_libraries(tactile-runtime
                      PUBLIC
                      tactile::base

                      PRIVATE
                      tactile::core
                      SDL2::SDL2
                      )
