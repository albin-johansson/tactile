project(tactile-opengl CXX)

set(TACTILE_OPENGL_DIR "${TACTILE_ROOT_DIR}/modules/opengl")

file(GLOB_RECURSE TACTILE_OPENGL_SOURCE_FILES
     CONFIGURE_DEPENDS
     "${TACTILE_OPENGL_DIR}/inc/**/*.hpp"
     "${TACTILE_OPENGL_DIR}/src/**/*.cpp"
     )

add_library(${TACTILE_OPENGL_TARGET} SHARED ${TACTILE_OPENGL_SOURCE_FILES})

tactile_prepare_target(${TACTILE_OPENGL_TARGET})

target_compile_definitions(${TACTILE_OPENGL_TARGET}
                           PRIVATE
                           TACTILE_BUILDING_OPENGL_PLUGIN
                           )

target_include_directories(${TACTILE_OPENGL_TARGET} PUBLIC "${TACTILE_OPENGL_DIR}/inc")

target_link_libraries(${TACTILE_OPENGL_TARGET}
                      PUBLIC
                      ${TACTILE_CORE_TARGET}

                      PRIVATE
                      SDL2::SDL2
                      glad::glad
                      imgui::imgui
                      )

install(TARGETS ${TACTILE_OPENGL_TARGET} DESTINATION "${TACTILE_BUILD_DIR}/plugins/renderers")
