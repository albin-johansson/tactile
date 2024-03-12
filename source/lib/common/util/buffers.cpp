// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "buffers.hpp"

#include <algorithm>  // find
#include <cstring>    // memset
#include <iterator>   // distance

#include "tactile/base/int.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

void zero_buffer(Span<char> buffer)
{
  std::memset(buffer.data(), 0, buffer.size_bytes());
}

void copy_string_into_buffer(Span<char> buffer, StringView str)
{
  zero_buffer(buffer);

  usize index = 0;
  for (const auto ch: str) {
    if (index < buffer.size()) {
      buffer[index] = ch;
    }
    else {
      // The string is larger than the buffer, so we are finished.
      break;
    }

    ++index;
  }
}

auto create_string_from_buffer(Span<const char> buffer) -> String
{
  return String {create_string_view_from_buffer(buffer)};
}

auto create_string_view_from_buffer(Span<const char> buffer) -> StringView
{
  auto iter = std::find(buffer.begin(), buffer.end(), '\0');
  if (iter != buffer.end()) {
    const auto index = static_cast<usize>(std::distance(buffer.begin(), iter));
    return StringView {buffer.data(), index};
  }
  else {
    throw Exception {"Invalid string buffer!"};
  }
}

}  // namespace tactile
