// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "tactile/core/container/array.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/primitives.hpp"

namespace tactile {

/// A character buffer that is safe to use as the output buffer with std::format_to.
template <usize Capacity>
class FormatBuffer final
{
 public:
  using value_type = char;
  using size_type = usize;

  /// Resets the buffer size (but doesn't clear the buffer content).
  void clear() noexcept
  {
    m_size = 0;
  }

  /// Adds a character to the buffer if it isn't full, does nothing otherwise.
  void push_back(const value_type value) noexcept
  {
    if (m_size < Capacity) [[likely]] {
      m_buffer[m_size] = value;
      ++m_size;
    }
  }

  /// Returns a view of the buffer content.
  auto view() const noexcept -> StringView
  {
    return StringView {m_buffer.data(), m_size};
  }

 private:
  Array<value_type, Capacity> m_buffer {};
  size_type m_size {0};
};

}  // namespace tactile
