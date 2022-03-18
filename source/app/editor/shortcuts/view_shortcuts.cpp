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

#include "view_shortcuts.hpp"

#include "editor/events/misc_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "mappings.hpp"

namespace tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : AShortcut{cen::scancodes::space, cen::key_mod::lshift}
{}

void CenterViewportShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CenterViewportEvent>();
}

auto CenterViewportShortcut::is_enabled(const DocumentModel& model,
                                        const WidgetManager&) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_MINUS}, primary_modifier}
{}

void DecreaseViewportZoomShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseZoomEvent>();
}

auto DecreaseViewportZoomShortcut::is_enabled(const DocumentModel& model,
                                              const WidgetManager&) const -> bool
{
  return model.can_decrease_viewport_tile_size();
}

/* ------------------------------------------------------------------------------------ */

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, primary_modifier}
{}

void IncreaseViewportZoomShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseZoomEvent>();
}

auto IncreaseViewportZoomShortcut::is_enabled(const DocumentModel& model,
                                              const WidgetManager&) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanUpShortcut::PanUpShortcut()
    : AShortcut{cen::scancodes::up, primary_modifier | cen::key_mod::lshift}
{}

void PanUpShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

auto PanUpShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, primary_modifier | cen::key_mod::lshift}
{}

void PanDownShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

auto PanDownShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanLeftShortcut::PanLeftShortcut()
    : AShortcut{cen::scancodes::left, primary_modifier | cen::key_mod::lshift}
{}

void PanLeftShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanRightShortcut::PanRightShortcut()
    : AShortcut{cen::scancodes::right, primary_modifier | cen::key_mod::lshift}
{}

void PanRightShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

auto PanRightShortcut::is_enabled(const DocumentModel& model, const WidgetManager&) const
    -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

ToggleGridShortcut::ToggleGridShortcut() : AShortcut{cen::scancodes::g, primary_modifier}
{}

void ToggleGridShortcut::activate(entt::dispatcher&)
{
  auto& prefs = get_preferences();
  prefs.set_grid_visible(!prefs.is_grid_visible());
}

/* ------------------------------------------------------------------------------------ */

ToggleLayerHighlightShortcut::ToggleLayerHighlightShortcut()
    : AShortcut{cen::scancodes::h}
{}

void ToggleLayerHighlightShortcut::activate(entt::dispatcher&)
{
  auto& prefs = get_preferences();
  prefs.set_highlight_active_layer(!prefs.highlight_active_layer());
}

/* ------------------------------------------------------------------------------------ */

ToggleUiShortcut::ToggleUiShortcut() : AShortcut{cen::scancodes::tab} {}

void ToggleUiShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ToggleUiEvent>();
}

auto ToggleUiShortcut::is_enabled(const DocumentModel& model,
                                  const WidgetManager& widgets) const -> bool
{
  return model.has_active_document() && widgets.is_editor_focused();
}

}  // namespace tactile