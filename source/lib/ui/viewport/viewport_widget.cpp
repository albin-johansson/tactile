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

#include "viewport_widget.hpp"

#include <centurion/keyboard.hpp>
#include <centurion/mouse_events.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "components/texture.hpp"
#include "components/viewport.hpp"
#include "model/event/map_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/language_system.hpp"
#include "ui/conversions.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/viewport/document_tab_widget.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"
#include "ui/widget/windows.hpp"

namespace tactile::ui {
namespace {

struct ViewportWidgetState final {
  bool has_focus = false;
  bool has_hover = false;
};

inline constinit ViewportWidgetState gWidgetState;

void _push_start_page(const Model& model, const Strings& strings, Dispatcher& dispatcher)
{
  prepare_vertical_alignment_center(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2 {0, 64});

  const auto& icons = model.get<Icons>();
  const auto& icon_texture = model.get<Texture>(icons.tactile_icon);

  center_next_item_horizontally(128);
  ImGui::Image(icon_texture.handle, {128, 128});

  ImGui::Spacing();
  ImGui::Spacing();

  if (push_centered_button(strings.action.create_map.c_str())) {
    dispatcher.enqueue<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();
  if (push_centered_button(strings.action.open_map.c_str())) {
    dispatcher.enqueue<ShowOpenMapDialogEvent>();
  }
}

}  // namespace

void show_viewport_dock(const Model& model, Entity, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  StyleVar padding {ImGuiStyleVar_WindowPadding, {4, 4}};
  remove_tab_bar_from_next_window();

  const Window window {"Viewport",
                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse};
  gWidgetState.has_focus = window.has_focus();
  gWidgetState.has_hover = window.is_hovered();

  if (window.is_open()) {
    padding.pop();

    if (sys::has_active_document(model)) {
      update_document_tabs(model, dispatcher);

      if (sys::is_map_document_active(model)) {
        if (window.mouse_entered()) {
          dispatcher.enqueue<ViewportMouseEnteredEvent>();
        }

        if (window.mouse_exited()) {
          dispatcher.enqueue<ViewportMouseExitedEvent>();
        }
      }
    }
    else {
      _push_start_page(model, strings, dispatcher);
    }
  }
}

void viewport_widget_mouse_wheel_event_handler(const Viewport& viewport,
                                               Dispatcher& dispatcher,
                                               const cen::mouse_wheel_event& event)
{
  if (cen::is_active(kPrimaryModifier)) {
    const auto y = event.precise_y();
    if (y > 0) {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }
    else if (y < 0 && viewport.can_zoom_out()) {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }
  }
  else {
    const Float2 precise {event.precise_x(), event.precise_y()};

    const float scaling = 4.0f;
    auto delta = precise * (viewport.tile_size / scaling);
    delta.x = -delta.x;

    dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
  }
}

auto is_viewport_focused() noexcept -> bool
{
  return gWidgetState.has_focus;
}

auto is_mouse_within_viewport() noexcept -> bool
{
  return gWidgetState.has_hover;
}

}  // namespace tactile::ui