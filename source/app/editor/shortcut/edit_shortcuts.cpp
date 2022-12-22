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

#include "editor/shortcut/mappings.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/document/map_document.hpp"
#include "model/event/command_events.hpp"
#include "model/event/component_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/event/view_events.hpp"
#include "model/model.hpp"
#include "model/tool/tool_manager.hpp"
#include "ui/menu/map_menu.hpp"
#include "ui/ui.hpp"

namespace tactile {

UndoShortcut::UndoShortcut()
    : Shortcut {cen::scancodes::z, primary_modifier}
{
}

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

RedoShortcut::RedoShortcut()
    : Shortcut {cen::scancodes::y, primary_modifier}
{
}

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

AddRowShortcut::AddRowShortcut()
    : Shortcut {cen::scancodes::r, secondary_modifier}
{
}

void AddRowShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddRowEvent>();
}

auto AddRowShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

AddColumnShortcut::AddColumnShortcut()
    : Shortcut {cen::scancodes::c, secondary_modifier}
{
}

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
    : Shortcut {cen::scancodes::r, secondary_modifier | cen::key_mod::lshift}
{
}

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
    : Shortcut {cen::scancodes::c, secondary_modifier | cen::key_mod::lshift}
{
}

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

AddTilesetShortcut::AddTilesetShortcut()
    : Shortcut {cen::scancodes::t, primary_modifier}
{
}

void AddTilesetShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
}

auto AddTilesetShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

EnableBucketShortcut::EnableBucketShortcut()
    : Shortcut {cen::scancodes::b}
{
}

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

EnableEraserShortcut::EnableEraserShortcut()
    : Shortcut {cen::scancodes::e}
{
}

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

EnableStampShortcut::EnableStampShortcut()
    : Shortcut {cen::scancodes::s}
{
}

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
    : Shortcut {cen::scancodes::q}
{
}

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

EnableRectangleToolShortcut::EnableRectangleToolShortcut()
    : Shortcut {cen::scancodes::r}
{
}

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

EnableEllipseToolShortcut::EnableEllipseToolShortcut()
    : Shortcut {cen::scancodes::t}
{
}

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

EnablePointToolShortcut::EnablePointToolShortcut()
    : Shortcut {cen::scancodes::y}
{
}

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
    : Shortcut {SDL_SCANCODE_COMMA, primary_modifier}
{
}

void OpenSettingsShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowSettingsEvent>();
}

/* ------------------------------------------------------------------------------------ */

OpenComponentEditorShortcut::OpenComponentEditorShortcut()
    : Shortcut {cen::scancodes::c, primary_modifier | cen::key_mod::lshift}
{
}

void OpenComponentEditorShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenComponentEditorEvent>();
}

auto OpenComponentEditorShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.is_map_active();
}

}  // namespace tactile
