cmake_minimum_required(VERSION 3.16)

project(tactile-private CXX)

function(tactile_set_properties target)
  set_target_properties(${target}
                        PROPERTIES
                        CXX_STANDARD 20
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        INTERPROCEDURAL_OPTIMIZATION ${TACTILE_USE_LTO}
                        )
endfunction()

function(tactile_set_compiler_warnings target)
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
                           )

    if (TACTILE_TREAT_WARNINGS_AS_ERRORS MATCHES ON)
      target_compile_options(${target} PRIVATE /WX)
    endif ()
  else ()
    target_compile_options(${target}
                           PRIVATE
                           -Wall
                           -Wextra
                           -Wpedantic
                           -Wconversion
                           -Wsign-conversion
                           )

    if (TACTILE_TREAT_WARNINGS_AS_ERRORS MATCHES ON)
      target_compile_options(${target} PRIVATE -Werror)
    endif ()
  endif ()
endfunction()

set(TACTILE_STANDARD_HEADERS
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

add_subdirectory(tactile/core)
