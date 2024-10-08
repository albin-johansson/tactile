// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t

#include <imgui.h>

#include "tactile/base/prelude.hpp"
#include "tactile/base/util/buffer.hpp"
#include "tactile/base/util/format.hpp"

namespace tactile {

template <std::size_t BufferSize, typename... Args>
void push_formatted_text(const FormatString<Args...> fmt, const Args&... args)
{
  Buffer<char, BufferSize> buffer;  // NOLINT: uninitialized

  format_to_buffer(buffer, fmt, args...);
  buffer.set_terminator('\0');

  ImGui::TextUnformatted(buffer.data());
}

}  // namespace tactile
