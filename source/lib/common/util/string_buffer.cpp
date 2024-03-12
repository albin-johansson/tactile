// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "string_buffer.hpp"

#include "common/util/buffers.hpp"

namespace tactile {

auto StringBuffer::operator=(StringView str) -> StringBuffer&
{
  copy_string_into_buffer(mBuffer, str);
  return *this;
}

void StringBuffer::clear()
{
  zero_buffer(mBuffer);
}

auto StringBuffer::as_string() const -> String
{
  return create_string_from_buffer(mBuffer);
}

auto StringBuffer::as_string_view() const -> StringView
{
  return create_string_view_from_buffer(mBuffer);
}

auto operator==(const StringBuffer& buffer, StringView str) -> bool
{
  return buffer.as_string_view() == str;
}

auto operator==(StringView str, const StringBuffer& buffer) -> bool
{
  return str == buffer.as_string_view();
}

}  // namespace tactile
