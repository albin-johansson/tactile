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

namespace tactile::ui {

struct DialogState;
struct RenderInfo;
struct ViewportCursorInfo;

class ADockWidget;
class ViewportArea;

class GraphicsCtx;

class StyleVar;
class StyleColor;
class Child;
class Combo;
class TabBar;
class TabItem;
class Table;
class Popup;
class ListBox;
class Menu;
class Modal;
class Window;
class TreeNode;

class ComponentEditor;

class ADialog;
class AStringInputDialog;
class AddComponentAttrDialog;
class ComponentNameDialog;
class DefineComponentDialog;
class RenameComponentAttributeDialog;
class RenameComponentDialog;
class AboutDialog;
class CreateMapDialog;
class CreditsDialog;
class MapParseErrorDialog;
class ResizeMapDialog;
class SettingsDialog;
class RenameLayerDialog;
class AddPropertyDialog;
class ChangePropertyTypeDialog;
class RenamePropertyDialog;
class CreateTilesetDialog;

class ContextMenu;
class AddLayerContextMenu;

}  // namespace tactile::ui