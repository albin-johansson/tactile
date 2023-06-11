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

#include "vk_error.hpp"

#include <fmt/format.h>
#include <magic_enum.hpp>

namespace tactile::vk {

VulkanError::VulkanError(String what, const VkResult res)
    : TactileError {fmt::format("{}: {}", what, to_string(res))}
{
}

auto to_string(const VkResult res) -> StringView
{
  return magic_enum::enum_name(res);
}

}  // namespace tactile::vk
