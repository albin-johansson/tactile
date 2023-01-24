/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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