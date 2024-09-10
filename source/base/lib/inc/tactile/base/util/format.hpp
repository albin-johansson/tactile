// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>      // size_t
#include <format>       // format_to_n, vformat_to, make_format_args, format_args
#include <iterator>     // back_inserter
#include <string>       // string
#include <string_view>  // string_view
#include <type_traits>  // type_identity_t
#include <utility>      // forward
#include <version>      // __cpp_lib_format

#include "tactile/base/prelude.hpp"
#include "tactile/base/util/buffer.hpp"

// Despite Clang having implemented most of std::format, they don't (yet)
// provide the associated version macros. Making it difficult to write portable
// version checks, so we have to do some guess work here.
#if TACTILE_COMPILER_CLANG && !defined(__cpp_lib_format)
  #define TACTILE_HAS_STD_FORMAT_STRING 0
#else
  #if __cpp_lib_format >= 202207L
    #define TACTILE_HAS_STD_FORMAT_STRING 1
  #else
    #define TACTILE_HAS_STD_FORMAT_STRING 0
  #endif
#endif

namespace tactile {

#if TACTILE_HAS_STD_FORMAT_STRING

template <typename... Args>
using FormatString = std::format_string<std::type_identity_t<Args>...>;

#else

template <typename... Args>
using FormatString = std::string_view;

#endif

/**
 * Formats a string into a given memory buffer.
 *
 * \details
 * The formatted string will be truncated if it doesn't fit in the buffer.
 *
 * \tparam N The maximum number of elements in the buffer.
 *
 * \param buffer The target memory buffer.
 * \param fmt    The format string.
 * \param args   The format arguments.
 */
template <std::size_t N>
void vformat_to_buffer(Buffer<char, N>& buffer,
                       const std::string_view fmt,
                       std::format_args args)
{
  const auto remaining_chars = buffer.capacity() - buffer.size();
  if (remaining_chars < 1) {
    return;
  }

  // We have to use vformat_to to avoid compile-time checks, because Clang might
  // actually feature such checks even though their library doesn't announce it.
  // This is safe because the Buffer::push_back function has no effect when the
  // buffer is full. Ideally, we would like to do something like:
  //     std::format_to_n(std::back_inserter(buffer),
  //                      saturate_cast<std::ptrdiff_t>(remaining_chars),
  //                      fmt,
  //                      std::forward<Args>(args)...);
  std::vformat_to(std::back_inserter(buffer), fmt, args);
}

/**
 * Formats a string into a given memory buffer.
 *
 * \details
 * The formatted string will be truncated if it doesn't fit in the buffer.
 *
 * \tparam N    The maximum number of elements in the buffer.
 * \tparam Args The format argument types.
 *
 * \param buffer The target memory buffer.
 * \param fmt    The format string.
 * \param args   The format arguments.
 */
template <std::size_t N, typename... Args>
void format_to_buffer(Buffer<char, N>& buffer,
                      const FormatString<Args...> fmt,
                      const Args&... args)
{
#if TACTILE_HAS_STD_FORMAT_STRING
  vformat_to_buffer(buffer, fmt.get(), std::make_format_args(args...));
#else
  vformat_to_buffer(buffer, fmt, std::make_format_args(args...));
#endif
}

}  // namespace tactile
