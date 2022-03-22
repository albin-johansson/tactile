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

#include "edit_shortcuts.hpp"

#include "core/map.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/component_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace tactile {

UndoShortcut::UndoShortcut() : AShortcut{cen::scancodes::z, primary_modifier} {}

void UndoShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<UndoEvent>();
}

auto UndoShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.can_undo();
}

/* ------------------------------------------------------------------------------------ */

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, primary_modifier} {}

void RedoShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RedoEvent>();
}

auto RedoShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.can_redo();
}

/* ------------------------------------------------------------------------------------ */

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, secondary_modifier} {}

void AddRowShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddRowEvent>();
}

auto AddRowShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

AddColumnShortcut::AddColumnShortcut() : AShortcut{cen::scancodes::c, secondary_modifier}
{}

void AddColumnShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddColumnEvent>();
}

auto AddColumnShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, secondary_modifier | cen::key_mod::lshift}
{}

void RemoveRowShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveRowEvent>();
}

auto RemoveRowShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  if (const auto* registry = model.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    return map.row_count > 1;
  }
  else {
    return false;
  }
}

/* ------------------------------------------------------------------------------------ */

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, secondary_modifier | cen::key_mod::lshift}
{}

void RemoveColumnShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveColumnEvent>();
}

auto RemoveColumnShortcut::is_enabled(const DocumentModel& model,
                                      const WidgetManager&) const -> bool
{
  if (const auto* registry = model.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    return map.column_count > 1;
  }
  else {
    return false;
  }
}

/* ------------------------------------------------------------------------------------ */

AddTilesetShortcut::AddTilesetShortcut() : AShortcut{cen::scancodes::t, primary_modifier}
{}

void AddTilesetShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
}

auto AddTilesetShortcut::is_enabled(const DocumentModel& model,
                                    const WidgetManager&) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b} {}

void EnableBucketShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::bucket);
}

auto EnableBucketShortcut::is_enabled(const DocumentModel& model,
                                      const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::bucket) && widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e} {}

void EnableEraserShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::eraser);
}

auto EnableEraserShortcut::is_enabled(const DocumentModel& model,
                                      const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::eraser) && widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s} {}

void EnableStampShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::stamp);
}

auto EnableStampShortcut::is_enabled(const DocumentModel& model,
                                     const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::stamp) && widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

EnableObjectSelectionShortcut::EnableObjectSelectionShortcut()
    : AShortcut{cen::scancodes::q}
{}

void EnableObjectSelectionShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::object_selection);
}

auto EnableObjectSelectionShortcut::is_enabled(const DocumentModel& model,
                                               const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::object_selection) &&
         widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

EnableRectangleToolShortcut::EnableRectangleToolShortcut() : AShortcut{cen::scancodes::r}
{}

void EnableRectangleToolShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::rectangle);
}

auto EnableRectangleToolShortcut::is_enabled(const DocumentModel& model,
                                             const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::rectangle) && widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

EnableEllipseToolShortcut::EnableEllipseToolShortcut() : AShortcut{cen::scancodes::t} {}

void EnableEllipseToolShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::ellipse);
}

auto EnableEllipseToolShortcut::is_enabled(const DocumentModel& model,
                                           const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::ellipse) && widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

EnablePointToolShortcut::EnablePointToolShortcut() : AShortcut{cen::scancodes::y} {}

void EnablePointToolShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::point);
}

auto EnablePointToolShortcut::is_enabled(const DocumentModel& model,
                                         const WidgetManager& widgets) const -> bool
{
  return model.is_tool_possible(ToolType::point) && widgets.is_editor_focused();
}

/* ------------------------------------------------------------------------------------ */

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{SDL_SCANCODE_COMMA, primary_modifier}
{}

void OpenSettingsShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowSettingsEvent>();
}

/* ------------------------------------------------------------------------------------ */

OpenComponentEditorShortcut::OpenComponentEditorShortcut()
    : AShortcut{cen::scancodes::c, primary_modifier | cen::key_mod::lshift}
{}

void OpenComponentEditorShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenComponentEditorEvent>();
}

}  // namespace tactile
