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

#include "core/common/filesystem.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"

namespace tactile {

struct ShowNewMapDialogEvent final
{};

struct ShowOpenMapDialogEvent final
{};

struct AddRowEvent final
{};

struct AddColumnEvent final
{};

struct RemoveRowEvent final
{};

struct RemoveColumnEvent final
{};

struct CreateMapEvent final
{
  Vector2i tile_size {};
  usize    row_count {};
  usize    column_count {};
};

struct OpenMapEvent final
{
  fs::path path;
};

struct InspectMapEvent final
{};

struct OpenResizeMapDialogEvent final
{};

struct ResizeMapEvent final
{
  usize row_count {};
  usize col_count {};
};

struct FixTilesInMapEvent final
{};

}  // namespace tactile
