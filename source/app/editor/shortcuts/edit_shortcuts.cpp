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

#include <entt/signal/dispatcher.hpp>

#include "core/commands/command_stack.hpp"
#include "core/documents/map_document.hpp"
#include "core/events/command_events.hpp"
#include "core/events/component_events.hpp"
#include "core/events/map_events.hpp"
#include "core/events/misc_events.hpp"
#include "core/events/tileset_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/model.hpp"
#include "core/tools/tool_manager.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/ui/menus/map_menu.hpp"
#include "editor/ui/ui.hpp"

namespace tactile {

UndoShortcut::UndoShortcut() : AShortcut{cen::scancodes::z, primary_modifier} {}

void UndoShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<UndoEvent>();
}

auto UndoShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  const auto* document = model.active_document();
  return document && document->get_history().can_undo();
}

/* ------------------------------------------------------------------------------------ */

RedoShortcut::RedoShortcut() : AShortcut{cen::scancodes::y, primary_modifier} {}

void RedoShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RedoEvent>();
}

auto RedoShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  const auto* document = model.active_document();
  return document && document->get_history().can_redo();
}

/* ------------------------------------------------------------------------------------ */

AddRowShortcut::AddRowShortcut() : AShortcut{cen::scancodes::r, secondary_modifier} {}

void AddRowShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddRowEvent>();
}

auto AddRowShortcut::is_enabled(const DocumentModel& model) const -> bool
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

auto AddColumnShortcut::is_enabled(const DocumentModel& model) const -> bool
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

auto RemoveRowShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      return model.view_map(id).get_map().row_count() > 1;
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

RemoveColumnShortcut::RemoveColumnShortcut()
    : AShortcut{cen::scancodes::c, secondary_modifier | cen::key_mod::lshift}
{}

void RemoveColumnShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RemoveColumnEvent>();
}

auto RemoveColumnShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      return model.view_map(id).get_map().column_count() > 1;
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

AddTilesetShortcut::AddTilesetShortcut() : AShortcut{cen::scancodes::t, primary_modifier}
{}

void AddTilesetShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
}

auto AddTilesetShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

EnableBucketShortcut::EnableBucketShortcut() : AShortcut{cen::scancodes::b} {}

void EnableBucketShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
}

auto EnableBucketShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::Bucket);
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e} {}

void EnableEraserShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
}

auto EnableEraserShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::Eraser);
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s} {}

void EnableStampShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
}

auto EnableStampShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::Stamp);
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

EnableObjectSelectionShortcut::EnableObjectSelectionShortcut()
    : AShortcut{cen::scancodes::q}
{}

void EnableObjectSelectionShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
}

auto EnableObjectSelectionShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::ObjectSelection);
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

EnableRectangleToolShortcut::EnableRectangleToolShortcut() : AShortcut{cen::scancodes::r}
{}

void EnableRectangleToolShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Rectangle);
}

auto EnableRectangleToolShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::Rectangle);
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

EnableEllipseToolShortcut::EnableEllipseToolShortcut() : AShortcut{cen::scancodes::t} {}

void EnableEllipseToolShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Ellipse);
}

auto EnableEllipseToolShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::Ellipse);
    }
  }

  return false;
}

/* ------------------------------------------------------------------------------------ */

EnablePointToolShortcut::EnablePointToolShortcut() : AShortcut{cen::scancodes::y} {}

void EnablePointToolShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Point);
}

auto EnablePointToolShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  if (ui::is_editor_focused() && model.has_active_document()) {
    const auto id = model.active_document_id().value();
    if (model.is_map(id)) {
      const auto& map = model.view_map(id);
      return map.get_tools().is_available(model, ToolType::Point);
    }
  }

  return false;
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

auto OpenComponentEditorShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.is_map_active();
}

}  // namespace tactile
