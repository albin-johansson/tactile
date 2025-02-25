
if (MSVC)
  list(APPEND
       TACTILE_COMPILE_OPTIONS
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
  list(APPEND
       TACTILE_COMPILE_OPTIONS
       -fvisibility=hidden
       -Wall
       -Wextra
       -Wpedantic
       -Wconversion
       -Wsign-conversion
       -Wswitch-enum
       -Wold-style-cast
       )

  if (TACTILE_BUILD_TYPE STREQUAL "asan")
    if (NOT MSVC)
      list(APPEND
           TACTILE_COMPILE_OPTIONS
           -fsanitize=address
           -fno-sanitize-recover
           -fno-omit-frame-pointer
           )
      list(APPEND
           TACTILE_LINK_OPTIONS
           -fsanitize=address
           -fno-sanitize-recover
           -fno-omit-frame-pointer
           )
    endif ()
  endif ()
endif ()

list(APPEND
     TACTILE_PRECOMPILED_HEADERS
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

message(DEBUG "TACTILE_COMPILE_OPTIONS: ${TACTILE_COMPILE_OPTIONS}")
message(DEBUG "TACTILE_LINK_OPTIONS: ${TACTILE_LINK_OPTIONS}")
message(DEBUG "TACTILE_PRECOMPILED_HEADERS: ${TACTILE_PRECOMPILED_HEADERS}")

function(tactile_set_output_directory target directory)
  set_target_properties("${target}"
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY "${directory}"
                        ARCHIVE_OUTPUT_DIRECTORY "${directory}"
                        LIBRARY_OUTPUT_DIRECTORY "${directory}"
                        )
endfunction()

function(tactile_prepare_target target)
  set_target_properties(${target}
                        PROPERTIES
                        NAMESPACE tactile
                        CXX_STANDARD 23
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        POSITION_INDEPENDENT_CODE ON
                        INTERPROCEDURAL_OPTIMIZATION ${TACTILE_USE_LTO}
                        PREFIX ""
                        )
  tactile_set_output_directory(${target} ${TACTILE_BUILD_DIR})
endfunction()

add_library(tactile_basic_target INTERFACE)

target_compile_options(tactile_basic_target INTERFACE ${TACTILE_COMPILE_OPTIONS})

target_link_options(tactile_basic_target INTERFACE ${TACTILE_LINK_OPTIONS})

if (TACTILE_USE_PRECOMPILED_HEADERS)
  target_precompile_headers(tactile_basic_target INTERFACE ${TACTILE_PRECOMPILED_HEADERS})
endif ()

if (TACTILE_BUILD_TYPE STREQUAL "asan" AND NOT MSVC)
  target_link_libraries(tactile_basic_target INTERFACE "-fsanitize=address")
endif ()

if (WIN32)
  target_compile_definitions(tactile_basic_target
                             INTERFACE
                             "WIN32_LEAN_AND_MEAN"
                             "NOMINMAX"
                             )
endif ()
