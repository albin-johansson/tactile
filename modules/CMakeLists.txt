project(tactile-modules CXX)

set(TACTILE_COMPILE_OPTIONS)
set(TACTILE_LINK_OPTIONS)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(TACTILE_COMPILE_OPTIONS
      /EHsc
      /MP
      /W4
      /bigobj
      /permissive-
      /Zc:preprocessor
      /Zc:__cplusplus
      /wd4251
      /wd4275
      )
else ()
  set(TACTILE_COMPILE_OPTIONS
      -fvisibility=hidden
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
      -Wswitch-enum
      )

  if (TACTILE_USE_SANITIZERS MATCHES ON)
    list(APPEND TACTILE_COMPILE_OPTIONS
         -fno-omit-frame-pointer
         -fsanitize=address
         )
    list(APPEND TACTILE_COMPILE_OPTIONS
         -fno-omit-frame-pointer
         -fsanitize=address
         )
  endif ()
endif ()

message(DEBUG "TACTILE_COMPILE_OPTIONS: ${TACTILE_COMPILE_OPTIONS}")
message(DEBUG "TACTILE_LINK_OPTIONS: ${TACTILE_LINK_OPTIONS}")

set(TACTILE_PRECOMPILED_HEADERS
    <algorithm>
    <array>
    <charconv>
    <chrono>
    <codecvt>
    <concepts>
    <cstddef>
    <cstdint>
    <cstring>
    <cstdlib>
    <ctime>
    <cmath>
    <deque>
    <exception>
    <filesystem>
    <fstream>
    <functional>
    <iterator>
    <limits>
    <locale>
    <map>
    <memory>
    <numbers>
    <optional>
    <ostream>
    <queue>
    <random>
    <span>
    <sstream>
    <string>
    <string_view>
    <system_error>
    <type_traits>
    <unordered_map>
    <utility>
    <variant>
    <vector>
    )

message(DEBUG "TACTILE_PRECOMPILED_HEADERS: ${TACTILE_PRECOMPILED_HEADERS}")

function(tactile_prepare_target target)
  set_target_properties(${target}
                        PROPERTIES
                        CXX_STANDARD 23
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        POSITION_INDEPENDENT_CODE ON
                        INTERPROCEDURAL_OPTIMIZATION ${TACTILE_USE_LTO}
                        RUNTIME_OUTPUT_DIRECTORY ${TACTILE_BUILD_DIR}
                        ARCHIVE_OUTPUT_DIRECTORY ${TACTILE_BUILD_DIR}
                        LIBRARY_OUTPUT_DIRECTORY ${TACTILE_BUILD_DIR}
                        )

  target_compile_options(${target} PRIVATE ${TACTILE_COMPILE_OPTIONS})
  target_link_options(${target} PRIVATE ${TACTILE_LINK_OPTIONS})
  target_precompile_headers(${target} PRIVATE ${TACTILE_PRECOMPILED_HEADERS})
endfunction()

function(tactile_set_output_dir target directory)
  set_target_properties(${target}
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY "${directory}"
                        ARCHIVE_OUTPUT_DIRECTORY "${directory}"
                        LIBRARY_OUTPUT_DIRECTORY "${directory}"
                        )
endfunction()

function(tactile_copy_directory_post_build target from to)
  add_custom_command(TARGET ${target} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${from}
                     ${to})
endfunction()

set(TACTILE_CORE_TARGET tactile-core)
set(TACTILE_EDITOR_TARGET tactile-editor)
set(TACTILE_TMJ_FORMAT_TARGET tactile-tmj-format)
set(TACTILE_TMX_FORMAT_TARGET tactile-tmx-format)
set(TACTILE_OPENGL_TARGET tactile-opengl)
set(TACTILE_VULKAN_TARGET tactile-vulkan)
set(TACTILE_MAIN_TARGET tactile)

add_subdirectory(core)
add_subdirectory(editor)
add_subdirectory(tmj-format)
add_subdirectory(opengl)
add_subdirectory(main)
