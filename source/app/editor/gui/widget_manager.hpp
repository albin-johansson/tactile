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

#include <memory>  // unique_ptr
#include <string>  // string

#include <entt/entt.hpp>

#include "core/attribute.hpp"
#include "io/maps/parser/parse_error.hpp"
#include "tactile.hpp"

namespace tactile {

class DocumentModel;
class IconManager;

class WidgetManager final
{
 public:
  TACTILE_DELETE_COPY(WidgetManager)
  TACTILE_DEFAULT_MOVE(WidgetManager)

  WidgetManager();

  ~WidgetManager() noexcept;

  void update(const DocumentModel& model,
              const IconManager& icons,
              entt::dispatcher& dispatcher);

  void show_settings();

  void show_new_map_dialog();

  void show_open_map_dialog();

  void show_add_tileset_dialog();

  void show_rename_layer_dialog(layer_id id);

  void show_add_property_dialog();

  void show_rename_property_dialog(const std::string& name);

  void show_change_property_type_dialog(std::string name, AttributeType type);

  void show_resize_map_dialog(usize currentRows, usize currentColumns);

  void show_map_import_error_dialog(parsing::ParseError error);

  void show_component_editor(const DocumentModel& model);

  void set_toolbar_visible(bool visible);

  [[nodiscard]] auto is_editor_focused() const -> bool;

  [[nodiscard]] auto is_toolbar_focused() const -> bool;

  [[nodiscard]] auto is_viewport_focused() const -> bool;

  [[nodiscard]] auto is_layer_dock_focused() const -> bool;

  [[nodiscard]] auto is_tileset_dock_focused() const -> bool;

  [[nodiscard]] auto is_property_dock_focused() const -> bool;

  [[nodiscard]] auto is_log_dock_focused() const -> bool;

  [[nodiscard]] auto is_tileset_dock_hovered() const -> bool;

  [[nodiscard]] auto is_toolbar_visible() const -> bool;

  [[nodiscard]] auto tileset_view_width() const -> Maybe<float>;

  [[nodiscard]] auto tileset_view_height() const -> Maybe<float>;

 private:
  struct Widgets;
  std::unique_ptr<Widgets> mWidgets;
};

}  // namespace tactile