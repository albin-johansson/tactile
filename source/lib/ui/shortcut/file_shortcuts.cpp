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

#include "file_shortcuts.hpp"

#include <entt/signal/dispatcher.hpp>

#include "model/cmd/command_stack.hpp"
#include "model/event/document_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/view_events.hpp"
#include "model/model.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/ui.hpp"

namespace tactile {

NewMapShortcut::NewMapShortcut()
    : Shortcut {cen::scancodes::n, kPrimaryModifier}
{
}

void NewMapShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowNewMapDialogEvent>();
}

/* ------------------------------------------------------------------------------------ */

OpenMapShortcut::OpenMapShortcut()
    : Shortcut {cen::scancodes::o, kPrimaryModifier}
{
}

void OpenMapShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowOpenMapDialogEvent>();
}

/* ------------------------------------------------------------------------------------ */

SaveShortcut::SaveShortcut()
    : Shortcut {cen::scancodes::s, kPrimaryModifier}
{
}

void SaveShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

auto SaveShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  const auto* document = model.active_document();
  return document && !document->get_history().is_clean();
}

/* ------------------------------------------------------------------------------------ */

SaveAsShortcut::SaveAsShortcut()
    : Shortcut {cen::scancodes::s, kPrimaryModifier | cen::key_mod::lshift}
{
}

void SaveAsShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenSaveAsDialogEvent>();
}

auto SaveAsShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

}  // namespace tactile
