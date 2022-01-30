cmake_minimum_required(VERSION 3.16)

project(tactile-app CXX)

find_package(OpenGL REQUIRED)  # Not acquired using vcpkg

find_package(imgui CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")

add_subdirectory(cfg)
add_subdirectory(core)
add_subdirectory(editor)
add_subdirectory(io)

set(SOURCE_FILES

    application.cpp
    application.hpp
    application_events.cpp
    application_events.hpp
    assert.cpp
    assert.hpp
    build.hpp
    logging.cpp
    logging.hpp
    profile.hpp
    tactile_def.hpp
    tactile_stdlib.hpp
    throw.hpp

    ${TACTILE_CFG_SOURCE_FILES}
    ${TACTILE_CORE_SOURCE_FILES}
    ${TACTILE_EDITOR_SOURCE_FILES}
    ${TACTILE_IO_SOURCE_FILES}
    )

add_library(${TACTILE_LIB} ${SOURCE_FILES})

add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})

target_include_directories(${TACTILE_LIB}
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}
                           ${ROOT_DIR}/modules/tactile-base

                           SYSTEM PUBLIC
                           ${LIBRARY_DIR}/centurion
                           ${LIBRARY_DIR}/fontawesome
                           ${STB_INCLUDE_DIRS}
                           ${Boost_INCLUDE_DIRS}
                           ${TACTILE_PROTO_GENERATED_DIR}
                           )

target_link_libraries(${TACTILE_LIB}
                      PUBLIC
                      ${TACTILE_PROTO}
                      SDL2::SDL2-static
                      SDL2::SDL2_image
                      imgui::imgui
                      EnTT::EnTT
                      magic_enum::magic_enum
                      fmt::fmt
                      protobuf::libprotobuf
                      yaml-cpp
                      nlohmann_json::nlohmann_json
                      pugixml::static
                      unofficial::nativefiledialog::nfd
                      GLEW::GLEW
                      OpenGL::GL
                      ${Boost_LIBRARIES}
                      )

target_compile_definitions(${TACTILE_LIB}
                           PUBLIC
                           CENTURION_NO_SDL_MIXER
                           CENTURION_NO_SDL_TTF
                           IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                           IMGUI_DEFINE_MATH_OPERATORS
                           GLEW_NO_GLU
                           BOOST_ENABLE_ASSERT_DEBUG_HANDLER
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           )

if (MSVC)
  target_compile_definitions(${TACTILE_LIB} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif ()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${TACTILE_EXE} WIN32 ${SOURCE_FILES} main.cpp)
else ()
  add_executable(${TACTILE_EXE} ${SOURCE_FILES} main.cpp)
endif ()

add_dependencies(${TACTILE_EXE} ${TACTILE_LIB})

tactile_set_compile_options(${TACTILE_LIB})
tactile_set_compile_options(${TACTILE_EXE})

target_precompile_headers(${TACTILE_LIB} PRIVATE
                          <cstdint>
                          <cstddef>
                          <memory>
                          <utility>
                          <array>
                          <vector>
                          <map>
                          <unordered_map>
                          <variant>
                          <string>
                          <string_view>
                          <concepts>
                          <functional>

                          <centurion.hpp>
                          <entt/entt.hpp>
                          <imgui.h>
                          <fmt/format.h>
                          <yaml-cpp/yaml.h>
                          <nlohmann/json.hpp>
                          <pugixml.hpp>
                          )

target_precompile_headers(${TACTILE_EXE} REUSE_FROM ${TACTILE_LIB})

target_link_libraries(${TACTILE_EXE}
                      PRIVATE
                      ${TACTILE_LIB}
                      SDL2::SDL2main
                      )

copy_directory_post_build(${TACTILE_EXE} ${RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)