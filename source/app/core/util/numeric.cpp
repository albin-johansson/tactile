/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "numeric.hpp"

#include <bit>  // endian

#include <boost/endian/conversion.hpp>

namespace tactile {

auto as_little_endian(const int32 value) noexcept -> int32
{
  return (std::endian::native == std::endian::little)
             ? value
             : boost::endian::endian_reverse(value);
}

auto as_little_endian(const uint32 value) noexcept -> uint32
{
  return (std::endian::native == std::endian::little)
             ? value
             : boost::endian::endian_reverse(value);
}

}  // namespace tactile