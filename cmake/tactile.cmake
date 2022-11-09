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
                           /wd4005 # Avoid macro redefinition warnings in third-party libraries
                           )

  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    target_compile_options(${target}
                           PRIVATE
                           -Wall
                           -Wextra
                           -Wpedantic
                           -Wconversion
                           -Wsign-conversion
                           )
  endif ()
endfunction()

function(tactile_enable_address_sanitizer)
  message(INFO "[Tactile]: Building with AddressSanitizer")
  add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
  add_link_options(-fno-omit-frame-pointer -fsanitize=address)
endfunction()

set(TACTILE_STD_HEADERS
    <algorithm>
    <array>
    <charconv>
    <chrono>
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