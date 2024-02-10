project(tactile-opengl-renderer CXX)

find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

set(TACTILE_OPENGL_TARGET tactile-opengl-renderer)

file(GLOB_RECURSE TACTILE_OPENGL_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/inc/**/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_OPENGL_TARGET} SHARED "${TACTILE_OPENGL_SOURCE_FILES}")
add_library(tactile::opengl-renderer ALIAS ${TACTILE_OPENGL_TARGET})

tactile_prepare_target(${TACTILE_OPENGL_TARGET})

target_compile_definitions(${TACTILE_OPENGL_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_OPENGL_RENDERER
                           )

target_include_directories(${TACTILE_OPENGL_TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(${TACTILE_OPENGL_TARGET}
                      PUBLIC
                      tactile::foundation

                      PRIVATE
                      OpenGL::GL
                      SDL2::SDL2
                      glad::glad
                      imgui::imgui
                      )

install(TARGETS ${TACTILE_OPENGL_TARGET} DESTINATION "${TACTILE_BUILD_DIR}/plugins/renderers")
