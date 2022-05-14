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

#include "core/common/identifiers.hpp"
#include "core/common/tile_cache.hpp"
#include "core/mouse_info.hpp"
#include "core/tile_pos.hpp"
#include "core/tools/tool_type.hpp"

namespace tactile {

/// \addtogroup events
/// \{

/**
 * \brief Event that is emitted when a mouse tool is pressed.
 */
struct ToolPressedEvent final
{
  MouseInfo info;
};

/**
 * \brief Event that is emitted when a mouse tool is dragged.
 */
struct ToolDraggedEvent final
{
  MouseInfo info;
};

/**
 * \brief Event that is emitted when a mouse tool is released.
 */
struct ToolReleasedEvent final
{
  MouseInfo info;
};

struct ToolEnteredEvent final
{};

struct ToolExitedEvent final
{};

struct SelectToolEvent final
{
  ToolType type{};
};

struct StampSequenceEvent final
{
  TileCache old_state;
  TileCache sequence;
};

struct SetStampRandomizerEvent final
{
  bool enabled{};  ///< Whether the randomizer mode is enabled.
};

struct EraserSequenceEvent final
{
  TileCache old_state;
};

struct FloodEvent final
{
  TilePos origin;
  TileID replacement{};
};

/**
 * \brief Emitted when a rectangle should be added to the current object layer.
 */
struct AddRectangleEvent final
{
  float x{};
  float y{};
  float width{};
  float height{};
};

/**
 * \brief Emitted when an ellipse should be added to the current object layer.
 */
struct AddEllipseEvent final
{
  float x{};
  float y{};
  float width{};
  float height{};
};

/**
 * \brief Emitted when a point should be added to the current object layer.
 */
struct AddPointEvent final
{
  float x{};
  float y{};
};

/// \} End of group events

}  // namespace tactile