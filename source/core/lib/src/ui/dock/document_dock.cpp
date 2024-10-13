// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/document_dock.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/events.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/canvas_overlay.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/overlays.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/render/orthogonal_renderer.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::core::ui {
namespace {

void _push_map_document_overlay(const Registry& registry,
                                const EntityID map_id,
                                const CanvasRenderer& canvas_renderer)
{
  if (const OverlayScope overlay {"##MapDocumentOverlay", Float2 {1.0f, 0.0f}, 0.5f};
      overlay.is_open()) {
    const auto& viewport = registry.get<CViewport>(map_id);

    push_viewport_info_section(viewport);
    push_canvas_info_section(canvas_renderer);
    push_viewport_mouse_info_section(canvas_renderer);
  }
}

void _push_document_tab(const IDocument& document, EventDispatcher& dispatcher)
{
  const auto& registry = document.get_registry();
  const auto& document_info = registry.get<CDocumentInfo>();

  const IdScope document_scope {document_info.root};

  if (const TabItemScope tab {document_info.name.c_str()}; tab.is_open()) {
    const auto canvas_tl = ImGui::GetCursorScreenPos();
    const auto canvas_br = canvas_tl + ImGui::GetContentRegionAvail();
    const auto canvas_size = canvas_br - canvas_tl;

    const auto& viewport = registry.get<CViewport>(document_info.root);
    if (viewport.size != to_float2(canvas_size)) {
      dispatcher.push<UpdateViewportSizeEvent>(document_info.root, to_float2(canvas_size));
    }

    const auto tile_size = document.get_tile_size();
    const auto extent = document.get_extent();

    const CanvasRenderer canvas_renderer {to_float2(canvas_tl),
                                          to_float2(canvas_br),
                                          extent,
                                          tile_size,
                                          viewport};

    if (is_map(registry, document_info.root)) {
      render_orthogonal_map(canvas_renderer, registry, document_info.root);
      _push_map_document_overlay(registry, document_info.root, canvas_renderer);
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
}

void _push_document_tabs(const Model& model, EventDispatcher& dispatcher)
{
  if (const TabBarScope tabs {"##TabBar"}; tabs.is_open()) {
    const auto& document_manager = model.get_document_manager();
    const auto& open_documents = document_manager.get_open_documents();

    for (const auto& document_uuid : open_documents) {
      const auto& document = document_manager.get_document(document_uuid);
      _push_document_tab(document, dispatcher);
    }
  }
}

void _push_empty_view(const Language& language, EventDispatcher& dispatcher)
{
  prepare_for_vertically_centered_widgets(2);

  if (push_horizontally_centered_button(language.get(ActionLabel::kCreateMap))) {
    dispatcher.push<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();

  if (push_horizontally_centered_button(language.get(ActionLabel::kOpenMap))) {
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

  const Window dock_window {language.get(NounLabel::kDocumentDock),
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

}  // namespace tactile::core::ui
