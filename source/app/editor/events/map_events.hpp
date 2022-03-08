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

#pragma once

#include <filesystem>  // path

#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct show_new_map_dialog_event final
{};

struct show_open_map_dialog_event final
{};

struct add_row_event final
{};

struct add_column_event final
{};

struct remove_row_event final
{};

struct remove_column_event final
{};

struct create_map_event final
{
  int32 tile_width{};
  int32 tile_height{};
  usize row_count{};
  usize column_count{};
};

struct close_map_event final
{
  map_id id{};
};

struct open_map_event final
{
  std::filesystem::path path;
};

struct select_map_event final
{
  map_id id;
};

struct inspect_map_event final
{};

struct open_resize_map_dialog_event final
{};

struct resize_map_event final
{
  usize row_count{};
  usize col_count{};
};

/// \} End of group events

}  // namespace tactile
