cmake_minimum_required(VERSION 3.16)

project(tactile-app CXX)

file(GLOB_RECURSE SOURCE_FILES
     CONFIGURE_DEPENDS
     **/*.cpp
     **/*.hpp
     *.cpp
     *.hpp
     )

add_library(${TACTILE_LIB} ${SOURCE_FILES})

add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})

target_include_directories(${TACTILE_LIB}
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}

                           SYSTEM PUBLIC
                           ${LIBRARY_DIR}/centurion
                           ${LIBRARY_DIR}/fontawesome
                           ${STB_INCLUDE_DIRS}
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
                      glm::glm
                      protobuf::libprotobuf
                      yaml-cpp
                      nlohmann_json::nlohmann_json
                      pugixml::static
                      tinyfiledialogs::tinyfiledialogs
                      GLEW::GLEW
                      OpenGL::GL
                      Boost::boost
                      Boost::container
                      Boost::exception
                      spdlog::spdlog
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
                           BOOST_USER_CONFIG="cfg/boost_cfg.hpp"
                           BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT
                           ENTT_STANDARD_CPP
                           SPDLOG_FMT_EXTERNAL
                           )

if (MSVC)
  target_compile_definitions(${TACTILE_LIB} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif ()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  message("[Tactile]: Building executable a WIN32 (non-console) executable...")
  add_executable(${TACTILE_EXE} WIN32 ${SOURCE_FILES} main.cpp)
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND TACTILE_BUILD_APP_BUNDLE MATCHES ON)
  message("[Tactile]: Building executable as application bundle...")

  file(GLOB_RECURSE TACTILE_BUNDLE_RESOURCES "${RESOURCE_DIR}/*")

  foreach (FILE ${TACTILE_BUNDLE_RESOURCES})
    file(RELATIVE_PATH NEW_FILE "${RESOURCE_DIR}/" ${FILE})
    get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
    set_property(SOURCE ${FILE}
                 PROPERTY MACOSX_PACKAGE_LOCATION "Resources/assets/${NEW_FILE_PATH}")
  endforeach ()

  # The app icon needs to be in the root Resources folder
  set_property(SOURCE "${RESOURCE_DIR}/Tactile.icns" PROPERTY MACOSX_PACKAGE_LOCATION "Resources/")

  add_executable(${TACTILE_EXE} MACOSX_BUNDLE ${SOURCE_FILES} main.cpp ${TACTILE_BUNDLE_RESOURCES})

  set_target_properties(${TACTILE_EXE} PROPERTIES
                        MACOSX_BUNDLE TRUE
                        MACOSX_BUNDLE_GUI_IDENTIFIER "com.albinjohansson.tactile"
                        MACOSX_BUNDLE_BUNDLE_NAME "Tactile"
                        MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}"
                        MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
                        MACOSX_BUNDLE_LONG_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
                        MACOSX_BUNDLE_COPYRIGHT "GPL-3.0"
                        MACOSX_BUNDLE_ICON_FILE "Tactile"
                        )
else ()
  add_executable(${TACTILE_EXE} ${SOURCE_FILES} main.cpp)
endif ()

add_dependencies(${TACTILE_EXE} ${TACTILE_LIB})

tactile_set_compile_options(${TACTILE_LIB})
tactile_set_compile_options(${TACTILE_EXE})

target_precompile_headers(${TACTILE_LIB} PRIVATE
                          <algorithm>
                          <array>
                          <charconv>
                          <chrono>
                          <concepts>
                          <cstddef>
                          <cstdint>
                          <cstring>
                          <cstdlib>
                          <cmath>
                          <deque>
                          <filesystem>
                          <fstream>
                          <functional>
                          <limits>
                          <map>
                          <memory>
                          <optional>
                          <random>
                          <span>
                          <string>
                          <string_view>
                          <system_error>
                          <type_traits>
                          <unordered_map>
                          <utility>
                          <variant>
                          <vector>

                          <imgui.h>
                          )

target_precompile_headers(${TACTILE_EXE} REUSE_FROM ${TACTILE_LIB})

target_link_libraries(${TACTILE_EXE}
                      PRIVATE
                      ${TACTILE_LIB}
                      SDL2::SDL2main
                      )

# Do not copy the resources when building as macOS app bundle
if (TACTILE_BUILD_APP_BUNDLE MATCHES OFF)
  copy_directory_post_build(${TACTILE_EXE} ${RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
endif ()