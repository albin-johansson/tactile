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

#include <IconsFontAwesome6.h>

#include "core/common/ints.hpp"
#include "core/layers/layer_type.hpp"
#include "core/utils/texture_manager.hpp"

/// \addtogroup gui
/// \{

/// \name Icon literals
/// \{

#define TAC_ICON_UNDO ICON_FA_ROTATE_LEFT
#define TAC_ICON_REDO ICON_FA_ROTATE_RIGHT

#define TAC_ICON_FILE ICON_FA_FILE_CIRCLE_PLUS
#define TAC_ICON_OPEN ICON_FA_FOLDER_OPEN
#define TAC_ICON_CLOSE ICON_FA_FILE_CIRCLE_XMARK
#define TAC_ICON_SELECT_FILE ICON_FA_ELLIPSIS
#define TAC_ICON_SAVE ICON_FA_FLOPPY_DISK
#define TAC_ICON_SETTINGS ICON_FA_GEAR
#define TAC_ICON_EXIT ICON_FA_ARROW_RIGHT_FROM_BRACKET

#define TAC_ICON_COPY ICON_FA_COPY
#define TAC_ICON_PASTE ICON_FA_PASTE

#define TAC_ICON_ADD ICON_FA_CIRCLE_PLUS
#define TAC_ICON_REMOVE ICON_FA_CIRCLE_MINUS
#define TAC_ICON_EDIT ICON_FA_PEN_TO_SQUARE
#define TAC_ICON_DUPLICATE ICON_FA_CLONE
#define TAC_ICON_RESIZE ICON_FA_EXPAND_ARROWS_ALT

#define TAC_ICON_MOVE_UP ICON_FA_ARROW_UP
#define TAC_ICON_MOVE_DOWN ICON_FA_ARROW_DOWN
#define TAC_ICON_MOVE_LEFT ICON_FA_ARROW_LEFT
#define TAC_ICON_MOVE_RIGHT ICON_FA_ARROW_RIGHT

#define TAC_ICON_INSPECT ICON_FA_CIRCLE_INFO
#define TAC_ICON_PROPERTIES ICON_FA_CODE
#define TAC_ICON_VISIBILITY ICON_FA_EYE
#define TAC_ICON_OPACITY ICON_FA_CHESS_BOARD

#define TAC_ICON_TILE_LAYER ICON_FA_TABLE_CELLS
#define TAC_ICON_OBJECTS ICON_FA_SHAPES
#define TAC_ICON_OBJECT_LAYER ICON_FA_SHAPES
#define TAC_ICON_GROUP_LAYER ICON_FA_LAYER_GROUP

#define TAC_ICON_METRICS ICON_FA_GAUGE_HIGH
#define TAC_ICON_ABOUT ICON_FA_CIRCLE_QUESTION

#define TAC_ICON_STAMP ICON_FA_STAMP
#define TAC_ICON_STAMP_RANDOMIZER ICON_FA_DICE
#define TAC_ICON_BUCKET ICON_FA_FILL
#define TAC_ICON_ERASER ICON_FA_ERASER
#define TAC_ICON_OBJECT_SELECTION ICON_FA_OBJECT_GROUP
#define TAC_ICON_RECTANGLE ICON_FA_SQUARE
#define TAC_ICON_ELLIPSE ICON_FA_CIRCLE
#define TAC_ICON_POINT ICON_FA_LOCATION_DOT

#define TAC_ICON_TILESET ICON_FA_IMAGE
#define TAC_ICON_GRID ICON_FA_TABLE_CELLS

#define TAC_ICON_CENTER ICON_FA_CROSSHAIRS
#define TAC_ICON_ZOOM_IN ICON_FA_MAGNIFYING_GLASS_PLUS
#define TAC_ICON_ZOOM_OUT ICON_FA_MAGNIFYING_GLASS_MINUS
#define TAC_ICON_RESET_ZOOM ICON_FA_MAGNIFYING_GLASS

#define TAC_ICON_LINK ICON_FA_LINK
#define TAC_ICON_BUG ICON_FA_BUG
#define TAC_ICON_ERROR ICON_FA_FACE_FROWN
#define TAC_ICON_REPAIR ICON_FA_SCREWDRIVER_WRENCH

#define TAC_ICON_HISTORY ICON_FA_CLOCK_ROTATE_LEFT
#define TAC_ICON_CLEAR_HISTORY ICON_FA_TRASH_CAN

#define TAC_ICON_RESET ICON_FA_SLIDERS
#define TAC_ICON_THREE_DOTS ICON_FA_ELLIPSIS

#define TAC_ICON_COMPONENT ICON_FA_CUBES

/// \} End of icon literals

/// \} End of group gui

namespace tactile::ui {

/**
 * \brief Loads some icons to be used in the GUI.
 */
void load_icons(TextureManager& textures);

/**
 * \brief Returns the texture identifier for the Tactile icon.
 *
 * \pre `load_icons()` must have been called before.
 *
 * \return a texture identifier.
 */
[[nodiscard]] auto get_tactile_icon() -> uint;

}  // namespace tactile::ui
