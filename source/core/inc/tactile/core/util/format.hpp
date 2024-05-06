// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <iterator>  // back_inserter
#include <utility>   // forward

#include <fmt/format.h>

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Formats a null-terminated string into a memory buffer.
 *
 * \details
 * Characters that don't fit in the provided memory buffer will be ignored.
 *
 * \tparam Args The types of the formatting arguments.
 *
 * \param buffer The destination memory buffer.
 * \param fmt    The format string.
 * \param args   The format arguments.
 */
template <typename... Args>
void format_cstr_to(fmt::memory_buffer& buffer,
                    fmt::format_string<Args...> fmt,
                    Args&&... args)
{
  const auto remaining_capacity = buffer.capacity() - buffer.size();
  if (remaining_capacity == 0) {
    return;
  }

  const auto char_limit = remaining_capacity - 1;
  fmt::format_to_n(std::back_inserter(buffer),
                   char_limit,
                   fmt,
                   std::forward<Args>(args)...);
  buffer.push_back('\0');
}

/**
 * Inserts a null-terminated string as a character array into a memory buffer.
 *
 * \details
 * The null-terminator will not be included in the memory buffer.
 *
 * \param buffer The destination memory buffer.
 * \param str    The C-style string that will be added to the memory buffer.
 */
void add_cstr_to(fmt::memory_buffer& buffer, const char* str);

}  // namespace tactile
