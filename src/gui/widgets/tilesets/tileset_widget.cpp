#include "tileset_widget.hpp"

#include <IconsFontAwesome5.h>

#include <algorithm>  // min, max

#include "core/events/add_tileset_event.hpp"
#include "core/events/remove_tileset_event.hpp"
#include "core/events/select_tileset_event.hpp"
#include "core/model.hpp"
#include "gui/get_texture_id.hpp"
#include "gui/show_grid.hpp"
#include "gui/widgets/button_ex.hpp"
#include "gui/widgets/menus/menu_bar_widget.hpp"
#include "gui/widgets/mouse_tracker.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "utils/scope_id.hpp"

namespace tactile {
namespace {

inline GridState state;
inline bool is_visible = true;

void ShowActiveTileset(const Tileset& tileset)
{
  state.grid_size = {static_cast<float>(tileset.GetTileWidth()),
                     static_cast<float>(tileset.GetTileHeight())};

  const auto texture = GetTextureID(tileset.GetTexture());
  const auto width = static_cast<float>(tileset.GetWidth());
  const auto height = static_cast<float>(tileset.GetHeight());

  const auto info = GetCanvasInfo();
  FillBackground(info);
  MouseTracker(info, state);

  state.scroll_offset.x = std::min(0.0f, state.scroll_offset.x);
  state.scroll_offset.y = std::min(0.0f, state.scroll_offset.y);

  state.scroll_offset.x =
      std::max(-width + info.canvas_size.x, state.scroll_offset.x);
  state.scroll_offset.y =
      std::max(-height + info.canvas_size.y, state.scroll_offset.y);

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->PushClipRect(info.canvas_tl, info.canvas_br, true);

  const auto position = drawList->GetClipRectMin() + state.scroll_offset;
  drawList->AddImage(texture, position, position + ImVec2{width, height});

  ShowGrid(state, info);
  drawList->PopClipRect();
}

void ShowTilesets(const MapDocument& document, entt::dispatcher& dispatcher)
{
  const auto& tilesets = document.GetTilesets();
  if (ImGui::BeginTabBar("TilesetTabBar", ImGuiTabBarFlags_Reorderable))
  {
    for (const auto& [id, tileset] : tilesets)
    {
      const ScopeID uid{id};

      bool opened = true;
      const auto isActive = tilesets.GetActiveTilesetId() == id;
      if (ImGui::BeginTabItem(tileset->GetName().data(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        ShowActiveTileset(*tileset);
        ImGui::EndTabItem();
      }

      if (!opened)
      {
        dispatcher.enqueue<RemoveTilesetEvent>(id);
      }
      else if (ImGui::IsItemActivated())
      {
        dispatcher.enqueue<SelectTilesetEvent>(id);
      }
    }

    ImGui::EndTabBar();
  }
}

void ShowNoTilesetsView()
{
  ImGui::Text("No tilesets available!");
}

}  // namespace

void UpdateTilesetWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible || !model.GetActiveMapId())
  {
    return;
  }

  const auto* document = model.GetActiveDocument();
  const auto& tilesets = document->GetTilesets();

  if (ImGui::Begin("Tilesets",
                   &is_visible,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Create tileset."))
    {
      EnableTilesetDialog();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE,
                 "Remove tileset.",
                 tilesets.HasActiveTileset()))
    {
      dispatcher.enqueue<RemoveTilesetEvent>(*tilesets.GetActiveTilesetId());
    }

    if (tilesets.GetSize() != 0)
    {
      ShowTilesets(*document, dispatcher);
    }
    else
    {
      ShowNoTilesetsView();
    }
  }

  ImGui::End();
}

void SetTilesetWidgetVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsTilesetWidgetVisible() noexcept -> bool
{
  return is_visible;
}

}  // namespace tactile
