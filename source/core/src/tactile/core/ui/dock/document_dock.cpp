// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/document_dock.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/dialog_events.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/viewport_events.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::ui {
namespace {

void _render_map(const Float2& canvas_tl,
                 const Float2& canvas_br,
                 const Registry& registry,
                 const EntityID map_id)
{
  const auto& map = registry.get<CMap>(map_id);
  const auto& viewport = registry.get<CViewport>(map_id);

  const CanvasRenderer canvas_renderer {canvas_tl,
                                        canvas_br,
                                        map.extent,
                                        map.tile_size,
                                        viewport};

  constexpr UColor bg_color {0x33, 0x33, 0x33, 0xFF};
  canvas_renderer.clear_canvas(bg_color);
  canvas_renderer.draw_orthogonal_grid(kColorBlack);
}

void _push_document_tab(const IDocument& document, EventDispatcher& dispatcher)
{
  const auto canvas_tl = ImGui::GetCursorScreenPos();
  const auto canvas_br = canvas_tl + ImGui::GetContentRegionAvail();
  const auto canvas_size = canvas_br - canvas_tl;

  const auto& registry = document.get_registry();
  const auto& document_info = registry.get<CDocumentInfo>();

  const auto& viewport = registry.get<CViewport>(document_info.root);
  if (viewport.size != to_float2(canvas_size)) {
    dispatcher.push<UpdateViewportSizeEvent>(document_info.root, to_float2(canvas_size));
  }

  if (is_map(registry, document_info.root)) {
    _render_map(to_float2(canvas_tl), to_float2(canvas_br), registry, document_info.root);
  }
  else {
    // TODO render tileset
  }

  ImGui::InvisibleButton("##Canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft);

  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.0f)) {
    const auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.0f);
    dispatcher.push<OffsetViewportEvent>(document_info.root, to_float2(-delta));
    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_8, ImGuiInputFlags_RouteFocused)) {
    dispatcher.push<ResetViewportZoomEvent>(document_info.root);
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_9,
                      ImGuiInputFlags_RouteFocused | ImGuiInputFlags_Repeat)) {
    dispatcher.push<DecreaseViewportZoomEvent>(document_info.root);
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_0,
                      ImGuiInputFlags_RouteFocused | ImGuiInputFlags_Repeat)) {
    dispatcher.push<IncreaseViewportZoomEvent>(document_info.root);
  }

  if (ImGui::Shortcut(ImGuiMod_Shift | ImGuiKey_Space, ImGuiInputFlags_RouteFocused)) {
    dispatcher.push<CenterViewportEvent>(document_info.root, document.get_content_size());
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_UpArrow,
                      ImGuiInputFlags_RouteFocused | ImGuiInputFlags_Repeat)) {
    dispatcher.push<PanViewportUpEvent>(document_info.root);
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_DownArrow,
                      ImGuiInputFlags_RouteFocused | ImGuiInputFlags_Repeat)) {
    dispatcher.push<PanViewportDownEvent>(document_info.root);
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_LeftArrow,
                      ImGuiInputFlags_RouteFocused | ImGuiInputFlags_Repeat)) {
    dispatcher.push<PanViewportLeftEvent>(document_info.root);
  }

  if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_RightArrow,
                      ImGuiInputFlags_RouteFocused | ImGuiInputFlags_Repeat)) {
    dispatcher.push<PanViewportRightEvent>(document_info.root);
  }
}

void _push_document_tabs(const Model& model, EventDispatcher& dispatcher)
{
  if (const TabBarScope tabs {"##TabBar"}; tabs.is_open()) {
    const auto& document_manager = model.get_document_manager();
    const auto& open_documents = document_manager.get_open_documents();

    for (const auto& document_uuid : open_documents) {
      const auto& document = document_manager.get_document(document_uuid);

      const auto& registry = document.get_registry();
      const auto& document_info = registry.get<CDocumentInfo>();

      const IdScope document_scope {document_info.root};
      if (const TabItemScope tab {document_info.name.c_str()}; tab.is_open()) {
        _push_document_tab(document, dispatcher);
      }
    }
  }
}

void _push_empty_view(const Language& language, EventDispatcher& dispatcher)
{
  prepare_for_vertically_centered_widgets(2);

  if (push_horizontally_centered_button(language.get(StringID::kCreateMap))) {
    dispatcher.push<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();

  if (push_horizontally_centered_button(language.get(StringID::kOpenMap))) {
    dispatcher.push<ShowOpenMapDialogEvent>();
  }
}

}  // namespace

void DocumentDock::push(const Model& model, EventDispatcher& dispatcher)
{
  ImGuiWindowClass window_class {};
  window_class.DockNodeFlagsOverrideSet =
      ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoDockingSplit |
      static_cast<ImGuiDockNodeFlags>(ImGuiDockNodeFlags_NoTabBar);

  ImGui::SetNextWindowClass(&window_class);

  const auto& language = model.get_language();

  const Window dock_window {language.get(StringID::kDocumentDock),
                            ImGuiWindowFlags_NoScrollbar};
  if (dock_window.is_open()) {
    const auto& document_manager = model.get_document_manager();
    const auto& open_documents = document_manager.get_open_documents();

    if (open_documents.empty()) {
      _push_empty_view(language, dispatcher);
    }
    else {
      _push_document_tabs(model, dispatcher);
    }
  }
}

}  // namespace tactile::ui
