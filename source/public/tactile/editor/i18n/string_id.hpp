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

#pragma once

#include "tactile/core/common/prelude.hpp"

namespace tactile {

enum class [[maybe_unused]] StringID : uint32 {
  // Menu names
  Menu_File,
  Menu_Edit,
  Menu_View,
  Menu_Map,
  Menu_Tileset,
  Menu_Help,
  Menu_Debug,
  Menu_RecentFiles,
  Menu_Widget,
  Menu_ExportAs,

  // Dock widget names
  Dock_Layer,
  Dock_Tileset,
  Dock_Property,
  Dock_Component,
  Dock_Log,
  Dock_Animation,

  // Dialog names
  Dialog_Settings,
  Dialog_ComponentEditor,
  Dialog_About,
  Dialog_NewMap,
  Dialog_NewTileset,
  Dialog_NewComponent,
  Dialog_NewProperty,
  Dialog_ResizeMap,
  Dialog_Credits,
  Dialog_ParseError,
  Dialog_ChangePropertyType,
  Dialog_RenameProperty,
  Dialog_RenameComponent,
  Dialog_RenameAttribute,
  Dialog_ExportAsGodotScene,

  // Standalone verbs
  Verb_Accept,  // "OK"
  Verb_Cancel,
  Verb_Apply,
  Verb_Create,
  Verb_Close,
  Verb_Change,
  Verb_Add,
  Verb_Rename,
  Verb_Export,
  Verb_Undo,
  Verb_Redo,

  Noun_Map,
  Noun_TileLayer,
  Noun_ObjectLayer,
  Noun_GroupLayer,
  Noun_Tileset,
  Noun_Tile,
  Noun_Row,
  Noun_Rows,
  Noun_RowCount,
  Noun_Column,
  Noun_Columns,
  Noun_ColumnCount,
  Noun_Width,
  Noun_Height,
  Noun_TileWidth,
  Noun_TileHeight,
  Noun_TileCount,
  Noun_Size,
  Noun_Opacity,
  Noun_Name,
  Noun_Tag,
  Noun_Type,
  Noun_Default,
  Noun_Cause,
  Noun_Preview,
  Noun_License,
  Noun_Repository,
  Noun_Dependency,
  Noun_Index,
  Noun_Rectangle,
  Noun_Ellipse,
  Noun_Point,
  Noun_StampTool,
  Noun_EraserTool,
  Noun_BucketTool,
  Noun_ObjectSelectorTool,
  Noun_RectangleTool,
  Noun_EllipseTool,
  Noun_PointTool,
  Noun_FirstTileID,
  Noun_LastTileID,
  Noun_LocalID,
  Noun_GlobalID,
  Noun_TileCompression,
  Noun_TileEncoding,
  Noun_PlainEncoding,
  Noun_CompressionLevel,
  Noun_PreferredFormat,
  Noun_PreferredTileWidth,
  Noun_PreferredTileHeight,
  Noun_CommandCapacity,
  Noun_Language,
  Noun_Theme,
  Noun_Saturation,
  Noun_ViewportBackgroundColor,
  Noun_GridColor,

  Adjective_Empty,
  Adjective_Embedded,
  Adjective_Visible,
  Adjective_Animated,

  Determiner_None,

  Type_String,
  Type_Int,
  Type_Int2,
  Type_Int3,
  Type_Int4,
  Type_Float,
  Type_Float2,
  Type_Float3,
  Type_Float4,
  Type_Bool,
  Type_Color,
  Type_Path,
  Type_ObjectRef,

  Command_AddMapRow,
  Command_AddMapRows,
  Command_AddMapColumn,
  Command_AddMapColumns,
  Command_RemoveMapRow,
  Command_RemoveMapRows,
  Command_RemoveMapColumn,
  Command_RemoveMapColumns,
  Command_FixInvalidTiles,
  Command_ResizeMap,
  Command_SetTileFormatEncoding,
  Command_SetTileFormatCompression,
  Command_AddLayer,
  Command_RemoveLayer,
  Command_DuplicateLayer,
  Command_RenameLayer,
  Command_SetLayerOpacity,
  Command_ShowLayer,
  Command_HideLayer,
  Command_MoveLayerUp,
  Command_MoveLayerDown,
  Command_CreateTileset,
  Command_RemoveTileset,
  Command_RenameTileset,
  Command_AddProperty,
  Command_RemoveProperty,
  Command_RenameProperty,
  Command_UpdateProperty,
  Command_ChangePropertyType,

  Max,  // NOT a string
};

}  // namespace tactile
