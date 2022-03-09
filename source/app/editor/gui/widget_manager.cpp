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

#include "widget_manager.hpp"

#include <utility>  // move

#include "components/component_dock.hpp"
#include "dialogs/map_parse_error_dialog.hpp"
#include "dialogs/resize_map_dialog.hpp"
#include "editor/model.hpp"
#include "icons.hpp"
#include "layers/layer_dock.hpp"
#include "layout/dock_space.hpp"
#include "log/log_dock.hpp"
#include "menus/menu_bar.hpp"
#include "properties/properties_dock.hpp"
#include "tilesets/tileset_dock.hpp"
#include "toolbar/toolbar.hpp"
#include "viewport/map_view.hpp"
#include "viewport/viewport_widget.hpp"

namespace tactile {

struct widget_manager::widgets final
{
  MenuBar mMenuBar;
  Toolbar mToolbar;
  TilesetDock mTilesetDock;
  layer_dock mLayerDock;
  PropertiesDock mPropertiesDock;
  component_dock mComponentDock;
  LogDock mLogDock;

  resize_map_dialog mResizeMapDialog;
  map_parse_error_dialog mMapParseErrorDialog;
};

widget_manager::widget_manager() : mWidgets{std::make_unique<widgets>()} {}

widget_manager::~widget_manager() noexcept = default;

void widget_manager::update(const DocumentModel& model,
                            const IconManager& icons,
                            entt::dispatcher& dispatcher)
{
  mWidgets->mMenuBar.update(model, dispatcher);
  UpdateDockSpace();

  if (model.has_active_document()) {
    mWidgets->mToolbar.Update(model, dispatcher);
    mWidgets->mLayerDock.update(model, dispatcher);
    mWidgets->mPropertiesDock.Update(model, dispatcher);
    mWidgets->mComponentDock.update(model, dispatcher);
  }

  update_viewport_widget(model, icons, dispatcher);

  if (const auto* registry = model.active_registry()) {
    update_map_view_object_context_menu(*registry, dispatcher);
    mWidgets->mTilesetDock.Update(*registry, dispatcher);
    mWidgets->mLogDock.Update();
  }

  mWidgets->mResizeMapDialog.update(model, dispatcher);
  mWidgets->mMapParseErrorDialog.update(model, dispatcher);
}

void widget_manager::show_settings()
{
  mWidgets->mMenuBar.show_settings_dialog();
}

void widget_manager::show_new_map_dialog()
{
  mWidgets->mMenuBar.show_map_creation_dialog();
}

void widget_manager::show_open_map_dialog()
{
  mWidgets->mMenuBar.show_open_map_dialog();
}

void widget_manager::show_add_tileset_dialog()
{
  mWidgets->mMenuBar.show_tileset_creation_dialog();
}

void widget_manager::show_rename_layer_dialog(const layer_id id)
{
  mWidgets->mLayerDock.show_rename_layer_dialog(id);
}

void widget_manager::show_add_property_dialog()
{
  mWidgets->mPropertiesDock.ShowAddPropertyDialog();
}

void widget_manager::show_rename_property_dialog(const std::string& name)
{
  mWidgets->mPropertiesDock.ShowRenamePropertyDialog(name);
}

void widget_manager::show_change_property_type_dialog(std::string name,
                                                      const AttributeType type)
{
  mWidgets->mPropertiesDock.ShowChangePropertyTypeDialog(std::move(name), type);
}

void widget_manager::show_resize_map_dialog(const usize currentRows,
                                            const usize currentColumns)
{
  mWidgets->mResizeMapDialog.show(currentRows, currentColumns);
}

void widget_manager::show_map_import_error_dialog(const parsing::parse_error error)
{
  mWidgets->mMapParseErrorDialog.show(error);
}

void widget_manager::show_component_editor(const DocumentModel& model)
{
  mWidgets->mMenuBar.show_component_editor(model);
}

void widget_manager::set_toolbar_visible(const bool visible)
{
  mWidgets->mToolbar.SetVisible(visible);
}

auto widget_manager::is_editor_focused() const -> bool
{
  return is_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_properties_dock_focused() ||
         is_log_dock_focused();
}

auto widget_manager::is_toolbar_focused() const -> bool
{
  return mWidgets->mToolbar.IsFocused();
}

auto widget_manager::is_viewport_focused() const -> bool
{
  return tactile::is_viewport_focused();
}

auto widget_manager::is_layer_dock_focused() const -> bool
{
  return mWidgets->mLayerDock.has_focus();
}

auto widget_manager::is_tileset_dock_focused() const -> bool
{
  return mWidgets->mTilesetDock.IsFocused();
}

auto widget_manager::is_properties_dock_focused() const -> bool
{
  return mWidgets->mPropertiesDock.IsFocused();
}

auto widget_manager::is_log_dock_focused() const -> bool
{
  return mWidgets->mLogDock.IsFocused();
}

auto widget_manager::is_tileset_dock_hovered() const -> bool
{
  return mWidgets->mTilesetDock.IsHovered();
}

auto widget_manager::is_toolbar_visible() const -> bool
{
  return mWidgets->mToolbar.IsVisible();
}

auto widget_manager::tileset_view_width() const -> Maybe<float>
{
  return mWidgets->mTilesetDock.GetTilesetView().GetWidth();
}

auto widget_manager::tileset_view_height() const -> Maybe<float>
{
  return mWidgets->mTilesetDock.GetTilesetView().GetHeight();
}

}  // namespace tactile