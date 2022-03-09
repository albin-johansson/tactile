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
#include "editor/events/map_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace tactile {

UndoShortcut::UndoShortcut() : AShortcut{cen::scancodes::z, gPrimaryModifier} {}

void UndoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<UndoEvent>();
}

auto UndoShortcut::IsEnabled(const DocumentModel& model, const widget_manager&) const
    -> bool
{
  return model.can_undo();
}

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, gPrimaryModifier} {}

void RedoShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RedoEvent>();
}

auto RedoShortcut::IsEnabled(const DocumentModel& model, const widget_manager&) const
    -> bool
{
  return model.can_redo();
}

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, gSecondaryModifier} {}

void AddRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddRowEvent>();
}

auto AddRowShortcut::IsEnabled(const DocumentModel& model, const widget_manager&) const
    -> bool
{
  return model.has_active_document();
}

AddColumnShortcut::AddColumnShortcut() : AShortcut{cen::scancodes::c, gSecondaryModifier}
{}

void AddColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddColumnEvent>();
}

auto AddColumnShortcut::IsEnabled(const DocumentModel& model,
                                  const widget_manager&) const -> bool
{
  return model.has_active_document();
}

RemoveRowShortcut::RemoveRowShortcut()
    : AShortcut{cen::scancodes::r, gSecondaryModifier | cen::key_mod::lshift}
{}

void RemoveRowShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveRowEvent>();
}

auto RemoveRowShortcut::IsEnabled(const DocumentModel& model,
                                  const widget_manager&) const -> bool
{
  if (const auto* registry = model.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    return map.row_count > 1;
  }
  else {
    return false;
  }
}

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, gSecondaryModifier | cen::key_mod::lshift}
{}

void RemoveColumnShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveColumnEvent>();
}

auto RemoveColumnShortcut::IsEnabled(const DocumentModel& model,
                                     const widget_manager&) const -> bool
{
  if (const auto* registry = model.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    return map.column_count > 1;
  }
  else {
    return false;
  }
}

AddTilesetShortcut::AddTilesetShortcut() : AShortcut{cen::scancodes::t, gPrimaryModifier}
{}

void AddTilesetShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
}

auto AddTilesetShortcut::IsEnabled(const DocumentModel& model,
                                   const widget_manager&) const -> bool
{
  return model.has_active_document();
}

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b} {}

void EnableBucketShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::bucket);
}

auto EnableBucketShortcut::IsEnabled(const DocumentModel& model,
                                     const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::bucket) && widgets.is_editor_focused();
}

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e} {}

void EnableEraserShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::eraser);
}

auto EnableEraserShortcut::IsEnabled(const DocumentModel& model,
                                     const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::eraser) && widgets.is_editor_focused();
}

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s} {}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::stamp);
}

auto EnableStampShortcut::IsEnabled(const DocumentModel& model,
                                    const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::stamp) && widgets.is_editor_focused();
}

EnableObjectSelectionShortcut::EnableObjectSelectionShortcut()
    : AShortcut{cen::scancodes::q}
{}

void EnableObjectSelectionShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::object_selection);
}

auto EnableObjectSelectionShortcut::IsEnabled(const DocumentModel& model,
                                              const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::object_selection) &&
         widgets.is_editor_focused();
}

EnableRectangleToolShortcut::EnableRectangleToolShortcut() : AShortcut{cen::scancodes::r}
{}

void EnableRectangleToolShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::rectangle);
}

auto EnableRectangleToolShortcut::IsEnabled(const DocumentModel& model,
                                            const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::rectangle) && widgets.is_editor_focused();
}

EnableEllipseToolShortcut::EnableEllipseToolShortcut() : AShortcut{cen::scancodes::t} {}

void EnableEllipseToolShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::ellipse);
}

auto EnableEllipseToolShortcut::IsEnabled(const DocumentModel& model,
                                          const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::ellipse) && widgets.is_editor_focused();
}

EnablePointToolShortcut::EnablePointToolShortcut() : AShortcut{cen::scancodes::y} {}

void EnablePointToolShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(tool_type::point);
}

auto EnablePointToolShortcut::IsEnabled(const DocumentModel& model,
                                        const widget_manager& widgets) const -> bool
{
  return model.is_tool_possible(tool_type::point) && widgets.is_editor_focused();
}

}  // namespace tactile
