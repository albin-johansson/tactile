// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/format.hpp"

namespace tactile {

void add_cstr_to(fmt::memory_buffer& buffer, const char* str)
{
  if (str == nullptr) {
    return;
  }

  for (const char* curr = str; *curr != '\0'; ++curr) {
    if (buffer.size() == buffer.capacity()) {
      break;
    }

    buffer.push_back(*curr);
  }
}

}  // namespace tactile
