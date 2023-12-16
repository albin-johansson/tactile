project(tactile-modules CXX)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  list(APPEND TACTILE_COMPILE_OPTIONS
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
  list(APPEND TACTILE_COMPILE_OPTIONS
       -fvisibility=hidden
       -Wall
       -Wextra
       -Wpedantic
       -Wconversion
       -Wsign-conversion
       -Wswitch-enum
       )

  if (TACTILE_BUILD_TYPE STREQUAL "asan")
    if (NOT MSVC)
      list(APPEND TACTILE_COMPILE_OPTIONS
           -fsanitize=address
           -fno-sanitize-recover
           -fno-omit-frame-pointer
           )
      list(APPEND TACTILE_LINK_OPTIONS
           -fsanitize=address
           -fno-sanitize-recover
           -fno-omit-frame-pointer
           )
    endif ()
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
                        CXX_STANDARD "${TACTILE_CXX_STANDARD}"
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        POSITION_INDEPENDENT_CODE ON
                        INTERPROCEDURAL_OPTIMIZATION "${TACTILE_USE_LTO}"
                        RUNTIME_OUTPUT_DIRECTORY "${TACTILE_BUILD_DIR}"
                        ARCHIVE_OUTPUT_DIRECTORY "${TACTILE_BUILD_DIR}"
                        LIBRARY_OUTPUT_DIRECTORY "${TACTILE_BUILD_DIR}"
                        )

  target_compile_options(${target} PRIVATE "${TACTILE_COMPILE_OPTIONS}")
  target_link_options(${target} PRIVATE "${TACTILE_LINK_OPTIONS}")

  target_precompile_headers(${target} PRIVATE "${TACTILE_PRECOMPILED_HEADERS}")

  if (TACTILE_BUILD_TYPE STREQUAL "asan" AND NOT MSVC)
    target_link_libraries(${target} PRIVATE -fsanitize=address)
  endif ()
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

# Base
add_subdirectory(foundation)

# Test utilities
if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(testutil)
endif ()

# Editor
add_subdirectory(proto)
add_subdirectory(core)

# Save formats
add_subdirectory(tactile-yml-format)
add_subdirectory(tiled-xml-format)
add_subdirectory(tiled-json-format)
add_subdirectory(godot-scene-format)

# Renderers
add_subdirectory(opengl)

# Main executable
add_subdirectory(main)
