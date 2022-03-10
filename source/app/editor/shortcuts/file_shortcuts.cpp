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

#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/gui/menus/edit_menu.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, gPrimaryModifier} {}

void NewMapShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowNewMapDialogEvent>();
}

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, gPrimaryModifier} {}

void OpenMapShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowOpenMapDialogEvent>();
}

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{SDL_SCANCODE_COMMA, gPrimaryModifier}
{}

void OpenSettingsShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ShowSettingsEvent>();
}

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, gPrimaryModifier} {}

void SaveShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

auto SaveShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.is_save_possible();
}

SaveAsShortcut::SaveAsShortcut()
    : AShortcut{cen::scancodes::s, gPrimaryModifier | cen::key_mod::lshift}
{}

void SaveAsShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenSaveAsDialogEvent>();
}

auto SaveAsShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.is_save_possible();
}

}  // namespace tactile
