#include "viewport_widget.hpp"

#include <format>         // format
#include <string>         // string
#include <unordered_map>  // unordered_map

#include "core/events/add_map_event.hpp"
#include "core/events/close_map_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/select_map_event.hpp"
#include "core/model.hpp"
#include "gui/show_grid.hpp"
#include "gui/widgets/mouse_tracker.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "render_map.hpp"

namespace tactile {
namespace {

inline constexpr auto border_color = IM_COL32(0x10, 0x10, 0x10, 0xFF);

inline bool show_grid = true;
inline bool center_viewport = false;
inline GridState state;
inline std::unordered_map<map_id, std::string> map_ids;
inline int next_map_suffix = 1;

void RenderActiveMap(const MapDocument& document)
{
  auto* drawList = ImGui::GetWindowDrawList();
  const auto info = GetCanvasInfo();

  FillBackground(info);
  MouseTracker(info, state);

  drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

  if (center_viewport)
  {
    const auto rowCount = static_cast<float>(document.GetRowCount().get());
    const auto colCount = static_cast<float>(document.GetColumnCount().get());

    const auto width = colCount * state.grid_size.x;
    const auto height = rowCount * state.grid_size.y;

    state.scroll_offset.x = (info.canvas_size.x - width) / 2.0f;
    state.scroll_offset.y = (info.canvas_size.y - height) / 2.0f;

    center_viewport = false;
  }

  if (show_grid)
  {
    ShowGrid(state, info);
  }

  const auto offset = info.canvas_tl + state.scroll_offset;
  RenderMap(document, drawList, offset, state.grid_size);

  drawList->PopClipRect();
}

void ValidateMapId(const map_id id)
{
  if (const auto it = map_ids.find(id); it == map_ids.end())
  {
    map_ids.try_emplace(id,
                        std::format("Map {}##{}", next_map_suffix, id.get()));
    ++next_map_suffix;
  }
}

void ShowMaps(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabBar("ViewportTabBar", ImGuiTabBarFlags_Reorderable))
  {
    for (const auto& [id, document] : model)
    {
      ValidateMapId(id);

      bool opened = true;
      const auto isActive = model.GetActiveMapId() == id;
      if (ImGui::BeginTabItem(map_ids.at(id).c_str(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        if (isActive)
        {
          RenderActiveMap(*document);
        }

        ImGui::EndTabItem();
      }

      if (!opened)
      {
        dispatcher.enqueue<CloseMapEvent>(id);
      }
      else if (ImGui::IsItemActivated())
      {
        dispatcher.enqueue<SelectMapEvent>(id);
      }
    }

    ImGui::EndTabBar();
  }
}

void ShowNoActiveMapContent(entt::dispatcher& dispatcher)
{
  ImGui::Indent(50);

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::Text("No active map");

  ImGui::Spacing();
  if (ImGui::Button("Create new map"))
  {
    dispatcher.enqueue<AddMapEvent>();
  }

  ImGui::Spacing();
  if (ImGui::Button("Open existing map"))
  {
    dispatcher.enqueue<OpenMapEvent>();
  }
}

}  // namespace

void UpdateViewportWidget(const Model& model, entt::dispatcher& dispatcher)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{4, 4});

  state.grid_size = {64, 64};

  const auto* document = model.GetActiveDocument();
  const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

  if (ImGui::Begin("Viewport", nullptr, flags))
  {
    if (model.GetActiveMapId())
    {
      ShowMaps(model, dispatcher);
    }
    else
    {
      ShowNoActiveMapContent(dispatcher);
    }
  }

  ImGui::PopStyleVar();
  ImGui::End();
}

void CenterViewport()
{
  center_viewport = true;
}

void SetViewportGridEnabled(const bool enabled) noexcept
{
  show_grid = enabled;
}

}  // namespace tactile