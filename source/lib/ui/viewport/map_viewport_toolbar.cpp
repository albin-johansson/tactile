// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "map_viewport_toolbar.hpp"

#include <concepts>  // invocable

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/event/command_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/tool/tool_manager.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"
#include "ui/widget/windows.hpp"

namespace tactile::ui {
namespace {

inline constexpr auto kToolbarWindowFlags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoMove;

inline constexpr uint32 kToolbarHighlightColor = IM_COL32(0, 180, 0, 255);

struct MapViewportToolbarState final {
  float width {};
  bool visible {};
  bool has_hover {};
  bool has_focus {};
};

inline constinit MapViewportToolbarState gToolbarState;

void prepare_window_position(const ImVec2& offset = {})
{
  const auto window_pos = ImGui::GetWindowPos();
  const ImVec2 pos {window_pos.x + offset.x + 6.0f,
                    window_pos.y + offset.y + 6.0f + ImGui::GetFrameHeightWithSpacing()};
  const ImVec2 pivot {0.0f, 0.0f};

  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
  ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
}

void tool_button(const DocumentModel& model,
                 const ToolManager& tools,
                 entt::dispatcher& dispatcher,
                 const char* icon,
                 const char* tooltip,
                 const ToolType tool)
{
  const auto selected = tools.is_enabled(tool);

  if (selected) {
    ImGui::PushStyleColor(ImGuiCol_Button, kToolbarHighlightColor);
  }

  if (ui_icon_button(icon, tooltip, tools.is_available(model, tool))) {
    dispatcher.enqueue<SelectToolEvent>(tool);
  }

  if (selected) {
    ImGui::PopStyleColor();
  }
}

void show_extra_toolbar(std::invocable auto callable)
{
  const auto& style = ImGui::GetStyle();
  prepare_window_position({gToolbarState.width + style.ItemInnerSpacing.x, 0});

  if (const Window extra {"##ToolbarWindowExtra", kToolbarWindowFlags}; extra.is_open()) {
    // Prevent other mouse events in the viewport by treating both toolbars as one
    if (!gToolbarState.has_hover) {
      gToolbarState.has_hover = ImGui::IsWindowHovered();
    }
    callable();
  }
}

}  // namespace

void update_map_viewport_toolbar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  remove_tab_bar_from_next_window();

  prepare_window_position();
  ImGui::SetNextWindowBgAlpha(0.75f);

  const StyleVar padding {ImGuiStyleVar_WindowPadding, {6, 6}};

  const auto& lang = get_current_language();
  const auto& map_document =
      model.get_map_document(model.get_active_document_id().value());
  const auto& tools = map_document.get_tools();

  if (const Window window {"##ToolbarWindow", kToolbarWindowFlags}; window.is_open()) {
    gToolbarState.visible = true;
    gToolbarState.has_hover = ImGui::IsWindowHovered();
    gToolbarState.has_focus = window.has_focus();
    gToolbarState.width = ImGui::GetWindowSize().x;

    const auto& commands = map_document.get_history();

    if (ui_icon_button(TAC_ICON_UNDO, lang.misc.undo.c_str(), commands.can_undo())) {
      dispatcher.enqueue<UndoEvent>();
    }

    if (ui_icon_button(TAC_ICON_REDO, lang.misc.redo.c_str(), commands.can_redo())) {
      dispatcher.enqueue<RedoEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    if (ui_icon_button(TAC_ICON_TILESET, lang.tooltip.create_tileset.c_str())) {
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
    }

    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_STAMP,
                lang.misc.stamp_tool.c_str(),
                ToolType::Stamp);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_ERASER,
                lang.misc.eraser_tool.c_str(),
                ToolType::Eraser);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_BUCKET,
                lang.misc.bucket_tool.c_str(),
                ToolType::Bucket);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_OBJECT_SELECTION,
                lang.misc.object_selection_tool.c_str(),
                ToolType::ObjectSelection);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_RECTANGLE,
                lang.misc.rectangle_tool.c_str(),
                ToolType::Rectangle);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_ELLIPSE,
                lang.misc.ellipse_tool.c_str(),
                ToolType::Ellipse);

    tool_button(model,
                tools,
                dispatcher,
                TAC_ICON_POINT,
                lang.misc.point_tool.c_str(),
                ToolType::Point);
  }
  else {
    gToolbarState.visible = false;
    gToolbarState.has_hover = false;
    gToolbarState.has_focus = false;
  }

  if (gToolbarState.visible && tools.is_enabled(ToolType::Stamp)) {
    show_extra_toolbar([&] {
      const auto& tools = map_document.get_tools();
      const auto selected = tools.is_stamp_random();

      if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, kToolbarHighlightColor);
      }

      const auto& map = map_document.get_map();
      if (ui_icon_button(TAC_ICON_STAMP_RANDOMIZER,
                         lang.tooltip.stamp_random_tile.c_str(),
                         map.is_stamp_randomizer_possible())) {
        dispatcher.enqueue<SetStampRandomizerEvent>(!selected);
      }

      if (selected) {
        ImGui::PopStyleColor();
      }
    });
  }
}

auto is_map_toolbar_visible() -> bool
{
  return gToolbarState.visible;
}

auto is_map_toolbar_hovered() -> bool
{
  return gToolbarState.has_hover;
}

auto is_map_toolbar_focused() -> bool
{
  return gToolbarState.has_focus;
}

}  // namespace tactile::ui
