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

#include "file_shortcuts.hpp"

#include <entt/signal/dispatcher.hpp>

#include "core/commands/command_stack.hpp"
#include "core/events/map_events.hpp"
#include "core/events/misc_events.hpp"
#include "core/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/ui/ui.hpp"

namespace tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, primary_modifier} {}

void NewMapShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowNewMapDialogEvent>();
}

/* ------------------------------------------------------------------------------------ */

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, primary_modifier} {}

void OpenMapShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowOpenMapDialogEvent>();
}

/* ------------------------------------------------------------------------------------ */

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, primary_modifier} {}

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
    : AShortcut{cen::scancodes::s, primary_modifier | cen::key_mod::lshift}
{}

void SaveAsShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenSaveAsDialogEvent>();
}

auto SaveAsShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

}  // namespace tactile
