#include "map_content_viewport.hpp"

#include "core/events/close_map_event.hpp"
#include "core/events/select_map_event.hpp"
#include "core/model.hpp"
#include "gui/canvas_info.hpp"
#include "gui/show_grid.hpp"
#include "gui/widgets/mouse_tracker.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "io/preferences.hpp"
#include "render_map.hpp"
#include "utils/scope_id.hpp"

namespace tactile {
namespace {

inline bool center_viewport = false;
inline GridState state;

void RenderActiveMap(const MapDocument& document)
{
  state.grid_size = {64, 64};

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

  if (prefs::GetShowGrid())
  {
    ShowGrid(state, info);
  }

  const auto offset = info.canvas_tl + state.scroll_offset;
  RenderMap(document, drawList, offset, state.grid_size);

  drawList->PopClipRect();
}

}  // namespace

void MapContentViewport(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabBar("MapViewportTabBar", ImGuiTabBarFlags_Reorderable))
  {
    for (const auto& [id, document] : model)
    {
      const ScopeID uid{id};

      bool opened = true;
      const auto isActive = model.GetActiveMapId() == id;
      if (ImGui::BeginTabItem(document->GetName().data(),
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

void CenterMapContentViewport()
{
  center_viewport = true;
}

}  // namespace tactile
