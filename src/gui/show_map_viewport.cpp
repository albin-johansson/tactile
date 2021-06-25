#include "show_map_viewport.hpp"

#include <format>         // format
#include <string>         // string
#include <unordered_map>  // unordered_map

#include "core/model.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "render_map.hpp"
#include "show_grid.hpp"

namespace tactile {
namespace {

inline constexpr auto viewport_button_flags =
    ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight;
inline constexpr auto border_color = IM_COL32(0x10, 0x10, 0x10, 0xFF);

inline bool show_grid = true;
inline bool center_viewport = false;
inline GridState state;
inline std::unordered_map<map_id, std::string> map_ids;
inline int next_map_suffix = 1;

void MouseTracker(const CanvasInfo& info)
{
  ImGui::InvisibleButton("MapViewportCanvas",
                         info.canvas_size,
                         viewport_button_flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
  {
    const auto& io = ImGui::GetIO();
    state.scroll_offset.x += io.MouseDelta.x;
    state.scroll_offset.y += io.MouseDelta.y;
  }
}

void RenderActiveMap(const MapDocument& document)
{
  auto* drawList = ImGui::GetWindowDrawList();
  const auto info = GetCanvasInfo();

  FillBackground(info);
  MouseTracker(info);

  drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

  if (center_viewport)
  {
    const auto rowCount = static_cast<float>(document.GetColumnCount().get());
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

}  // namespace

void ShowMapViewport(const Model& model)
{
  constexpr auto flags = ImGuiWindowFlags_NoTitleBar;

  state.grid_size = {64, 64};

  const auto* document = model.GetActiveDocument();
  if (ImGui::Begin("MapViewport", nullptr, flags))
  {
    if (ImGui::BeginTabBar("ViewportTab"))
    {
      for (const auto& [id, document] : model)
      {
        ValidateMapId(id);

        const auto& key = map_ids.at(id);
        if (ImGui::BeginTabItem(key.c_str()))
        {
          if (model.GetActiveMapId() == id)
          {
            RenderActiveMap(*document);
          }

          ImGui::EndTabItem();
        }
      }

      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}

void CenterMapViewport()
{
  center_viewport = true;
}

void SetMapViewportGridEnabled(const bool enabled) noexcept
{
  show_grid = enabled;
}

}  // namespace tactile