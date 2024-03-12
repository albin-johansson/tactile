// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "viewport_widget.hpp"

#include <centurion/keyboard.hpp>
#include <centurion/mouse_events.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/viewport.hpp"
#include "document_tab_widget.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/model.hpp"
#include "ui/conversions.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
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

void update_start_page(entt::dispatcher& dispatcher)
{
  prepare_vertical_alignment_center(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2 {0, 64});

  center_next_item_horizontally(128);
  ImGui::Image(to_imgui_texture_id(get_tactile_icon()), {128, 128});

  ImGui::Spacing();
  ImGui::Spacing();

  const auto& lang = get_current_language();

  if (ui_centered_button(lang.action.create_map.c_str())) {
    dispatcher.enqueue<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();
  if (ui_centered_button(lang.action.open_map.c_str())) {
    dispatcher.enqueue<ShowOpenMapDialogEvent>();
  }
}

}  // namespace

void update_viewport_widget(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  StyleVar padding {ImGuiStyleVar_WindowPadding, {4, 4}};
  remove_tab_bar_from_next_window();

  const Window window {"Viewport",
                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse};
  gWidgetState.has_focus = window.has_focus();
  gWidgetState.has_hover = window.is_hovered();

  if (window.is_open()) {
    padding.pop();

    if (model.has_active_document()) {
      update_document_tabs(model, dispatcher);

      if (model.is_map_active()) {
        if (window.mouse_entered()) {
          dispatcher.enqueue<ViewportMouseEnteredEvent>();
        }

        if (window.mouse_exited()) {
          dispatcher.enqueue<ViewportMouseExitedEvent>();
        }
      }
    }
    else {
      update_start_page(dispatcher);
    }
  }
}

void viewport_widget_mouse_wheel_event_handler(const Viewport& viewport,
                                               entt::dispatcher& dispatcher,
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
    auto delta = precise * (viewport.tile_size() * (1.0f / scaling));
    delta[0] = -delta[0];

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
