function(copy_directory_post_build target from to)
  add_custom_command(TARGET ${target} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${from}
                     ${to})
endfunction()

function(tactile_set_compile_options target)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${target}
                           PRIVATE
                           /EHsc
                           /MP
                           /W4
                           /bigobj
                           /permissive-
                           /Zc:preprocessor
                           /Zc:__cplusplus
                           /wd4996  # No deprecation warnings
                           /wd4127  # No conditional expressions are constant warnings
                           )

    if (TACTILE_TREAT_WARNINGS_AS_ERRORS MATCHES ON)
      target_compile_options(${target} PRIVATE /WX)
    endif ()
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    target_compile_options(${target}
                           PRIVATE
                           -Wall
                           -Wextra
                           -Wpedantic
                           -Wconversion
                           -Wsign-conversion
                           -Wno-deprecated-declarations
                           )

    if (TACTILE_TREAT_WARNINGS_AS_ERRORS MATCHES ON)
      target_compile_options(${target} PRIVATE
                             -Werror
                             -Wno-error=unused-function
                             )
    endif ()
  endif ()
endfunction()

# Include a system directory (which suppresses its warnings).
# Taken from https://github.com/cpp-best-practices/cmake_template
function(tactile_target_include_system_directories target)
  set(multiValueArgs INTERFACE PUBLIC PRIVATE)
  cmake_parse_arguments(ARG
                        ""
                        ""
                        "${multiValueArgs}"
                        ${ARGN})
  foreach (scope IN ITEMS INTERFACE PUBLIC PRIVATE)
    foreach (lib_include_dirs IN LISTS ARG_${scope})
      if (NOT MSVC)
        # System includes do not work in MSVC
        # Awaiting:
        #   - https://gitlab.kitware.com/cmake/cmake/-/issues/18272
        #   - https://gitlab.kitware.com/cmake/cmake/-/issues/17904
        set(_SYSTEM SYSTEM)
      endif ()
      if (${scope} STREQUAL "INTERFACE" OR ${scope} STREQUAL "PUBLIC")
        target_include_directories(${target}
                                   ${_SYSTEM}
                                   ${scope}
                                   "$<BUILD_INTERFACE:${lib_include_dirs}>"
                                   "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
      else ()
        target_include_directories(${target}
                                   ${_SYSTEM}
                                   ${scope}
                                   ${lib_include_dirs})
      endif ()
    endforeach ()
  endforeach ()
endfunction()

# Include the directories of a library target as system directories (which suppresses their warnings).
# Taken from https://github.com/cpp-best-practices/cmake_template
function(tactile_target_include_system_library target scope lib)
  # Check if this is a target
  if (TARGET ${lib})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    if (lib_include_dirs)
      target_include_system_directories(${target} ${scope} ${lib_include_dirs})
    else ()
      message(TRACE "${lib} library does not have a INTERFACE_INCLUDE_DIRECTORIES property.")
    endif ()
  endif ()
endfunction()

# Link a library target as a system library (which suppresses its warnings).
# Taken from https://github.com/cpp-best-practices/cmake_template
function(tactile_target_link_system_library target scope lib)
  # Include the directories in the library
  target_include_system_library(${target} ${scope} ${lib})

  # Link the library
  target_link_libraries(${target} ${scope} ${lib})
endfunction()

# Link multiple library targets as system libraries (which suppresses their warnings).
# Taken from https://github.com/cpp-best-practices/cmake_template
function(target_link_system_libraries target)
  set(multiValueArgs INTERFACE PUBLIC PRIVATE)
  cmake_parse_arguments(ARG
                        ""
                        ""
                        "${multiValueArgs}"
                        ${ARGN})
  foreach (scope IN ITEMS INTERFACE PUBLIC PRIVATE)
    foreach (lib IN LISTS ARG_${scope})
      target_link_system_library(${target} ${scope} ${lib})
    endforeach ()
  endforeach ()
endfunction()

set(TACTILE_STD_HEADERS
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

set(TACTILE_PRECOMPILED_HEADERS ${TACTILE_STD_HEADERS})
