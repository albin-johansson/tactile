// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <format>    // format_to_n, format_string
#include <iterator>  // back_inserter
#include <utility>   // forward

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/sign_cast.hpp"
#include "tactile/core/util/memory_buffer.hpp"

namespace tactile {

/**
 * Formats a string into a given memory buffer.
 *
 * \details
 * The formatted string will be truncated if it doesn't fit in the memory
 * buffer, and a NUL-terminator is automatically included.
 *
 * \tparam N    The maximum number of elements in the buffer.
 * \tparam Args The format argument types.
 *
 * \param buffer The target memory buffer.
 * \param fmt    The format string.
 * \param args   The format arguments.
 */
template <usize N, typename... Args>
void format_to_buffer(MemoryBuffer<char, N>& buffer,
                      const std::format_string<Args...> fmt,
                      Args&&... args)
{
  const auto remaining_chars = buffer.remaining_capacity();
  if (remaining_chars < 1) {
    return;
  }

  const auto char_limit = remaining_chars - 1;
  std::format_to_n(std::back_inserter(buffer),
                   to_signed(char_limit),
                   fmt,
                   std::forward<Args>(args)...);
  buffer.push_back('\0');
}

}  // namespace tactile
