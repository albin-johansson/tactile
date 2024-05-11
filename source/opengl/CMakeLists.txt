project(tactile-opengl CXX)

file(GLOB_RECURSE TACTILE_OPENGL_SOURCE_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(tactile-opengl SHARED EXCLUDE_FROM_ALL)
add_library(tactile::opengl ALIAS tactile-opengl)

target_sources(tactile-opengl PRIVATE "${TACTILE_OPENGL_SOURCE_FILES}")

tactile_prepare_target(tactile-opengl)

target_compile_definitions(tactile-opengl PRIVATE "TACTILE_BUILDING_OPENGL_RENDERER")

target_include_directories(tactile-opengl
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           SYSTEM PRIVATE
                           "${Stb_INCLUDE_DIR}"
                           )

target_link_libraries(tactile-opengl
                      PUBLIC
                      tactile::base
                      tactile::render

                      PRIVATE
                      OpenGL::GL
                      glad::glad
                      SDL2::SDL2
                      imgui::imgui
                      )
