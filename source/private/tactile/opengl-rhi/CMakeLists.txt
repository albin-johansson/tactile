cmake_minimum_required(VERSION 3.16)

project(tactile-opengl-rhi CXX)

file(GLOB_RECURSE TACTILE_OPENGL_RHI_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_INCLUDE_DIR}/tactile/opengl-rhi/*.hpp
     ${TACTILE_INCLUDE_DIR}/tactile/opengl-rhi/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     ${PROJECT_SOURCE_DIR}/**.cpp
     )

add_library(${TACTILE_OPENGL_RHI} SHARED ${TACTILE_OPENGL_RHI_SOURCE_FILES})

tactile_set_properties(${TACTILE_OPENGL_RHI})
tactile_set_compiler_warnings(${TACTILE_OPENGL_RHI})

target_include_directories(${TACTILE_OPENGL_RHI} PUBLIC ${TACTILE_INCLUDE_DIR})

target_compile_definitions(${TACTILE_OPENGL_RHI}
                           PRIVATE
                           TACTILE_BUILDING_OPENGL_RHI
                           )

target_precompile_headers(${TACTILE_OPENGL_RHI} PRIVATE ${TACTILE_STANDARD_HEADERS})

target_link_libraries(${TACTILE_OPENGL_RHI}
                      PRIVATE
                      ${TACTILE_CORE}
                      OpenGL::GL
                      glad::glad
                      )