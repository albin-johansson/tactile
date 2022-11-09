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

#include <entt/signal/dispatcher.hpp>
#include <imgui_internal.h>

#include "core/viewport.hpp"
#include "editor/shortcut/mappings.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/ui.hpp"
#include "io/proto/preferences.hpp"
#include "model/event/view_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"

namespace tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : Shortcut {cen::scancodes::space, cen::key_mod::lshift}
{
}

void CenterViewportShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CenterViewportEvent>();
}

auto CenterViewportShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : Shortcut {cen::scan_code {SDLK_MINUS}, primary_modifier}
{
}

void DecreaseViewportZoomShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseZoomEvent>();
}

auto DecreaseViewportZoomShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  const auto* document = model.active_document();
  return document && document->get_viewport().can_zoom_out();
}

/* ------------------------------------------------------------------------------------ */

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : Shortcut {cen::scan_code {SDLK_PLUS}, primary_modifier}
{
}

void IncreaseViewportZoomShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseZoomEvent>();
}

auto IncreaseViewportZoomShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

IncreaseFontSizeShortcut::IncreaseFontSizeShortcut()
    : Shortcut {cen::scan_code {SDLK_PLUS}, primary_modifier | cen::key_mod::lshift}
{
}

void IncreaseFontSizeShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseFontSizeEvent>();
}

auto IncreaseFontSizeShortcut::is_enabled(const DocumentModel&) const -> bool
{
  /* Check for modals to avoid case of changing font size when settings dialog is open */
  return !ImGui::GetTopMostPopupModal() && ui::can_increase_font_size();
}

/* ------------------------------------------------------------------------------------ */

DecreaseFontSizeShortcut::DecreaseFontSizeShortcut()
    : Shortcut {cen::scan_code {SDLK_MINUS}, primary_modifier | cen::key_mod::lshift}
{
}

void DecreaseFontSizeShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseFontSizeEvent>();
}

auto DecreaseFontSizeShortcut::is_enabled(const DocumentModel&) const -> bool
{
  /* Check for modals to avoid case of changing font size when settings dialog is open */
  return !ImGui::GetTopMostPopupModal() && ui::can_decrease_font_size();
}

/* ------------------------------------------------------------------------------------ */

PanUpShortcut::PanUpShortcut()
    : Shortcut {cen::scancodes::up, primary_modifier | cen::key_mod::lshift}
{
}

void PanUpShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

auto PanUpShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanDownShortcut::PanDownShortcut()
    : Shortcut {cen::scancodes::down, primary_modifier | cen::key_mod::lshift}
{
}

void PanDownShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

auto PanDownShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanLeftShortcut::PanLeftShortcut()
    : Shortcut {cen::scancodes::left, primary_modifier | cen::key_mod::lshift}
{
}

void PanLeftShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

PanRightShortcut::PanRightShortcut()
    : Shortcut {cen::scancodes::right, primary_modifier | cen::key_mod::lshift}
{
}

void PanRightShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

auto PanRightShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document();
}

/* ------------------------------------------------------------------------------------ */

ToggleGridShortcut::ToggleGridShortcut()
    : Shortcut {cen::scancodes::g, primary_modifier}
{
}

void ToggleGridShortcut::activate(entt::dispatcher&)
{
  auto& prefs = io::get_preferences();
  prefs.show_grid = !prefs.show_grid;
}

/* ------------------------------------------------------------------------------------ */

ToggleLayerHighlightShortcut::ToggleLayerHighlightShortcut()
    : Shortcut {cen::scancodes::h}
{
}

void ToggleLayerHighlightShortcut::activate(entt::dispatcher&)
{
  auto& prefs = io::get_preferences();
  prefs.highlight_active_layer = !prefs.highlight_active_layer;
}

auto ToggleLayerHighlightShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.is_map_active();
}

/* ------------------------------------------------------------------------------------ */

ToggleUiShortcut::ToggleUiShortcut()
    : Shortcut {cen::scancodes::tab}
{
}

void ToggleUiShortcut::activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ToggleUiEvent>();
}

auto ToggleUiShortcut::is_enabled(const DocumentModel& model) const -> bool
{
  return model.has_active_document() && ui::is_editor_focused();
}

}  // namespace tactile