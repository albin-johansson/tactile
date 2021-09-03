#include "map_content_overlay.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/layer.hpp"
#include "core/components/tile_layer.hpp"
#include "core/systems/tile_layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "gui/rendering/canvas.hpp"
#include "viewport_cursor_info.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                       ImGuiWindowFlags_AlwaysAutoResize |
                       ImGuiWindowFlags_NoSavedSettings |
                       ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                       ImGuiWindowFlags_NoMove;

void MouseCoordinateLabels(const ViewportCursorInfo& cursor)
{
  if (ImGui::IsMousePosValid())
  {
    ImGui::Text("X: %.1f, Y: %.1f", cursor.raw_position.x, cursor.raw_position.y);
  }
  else
  {
    ImGui::TextUnformatted("X: N/A, Y: N/A");
  }
}

void MouseRowColumnLabels(const ViewportCursorInfo& cursor)
{
  if (cursor.is_within_map)
  {
    ImGui::Text("Row: %i, Column: %i",
                cursor.map_position.GetRow().get(),
                cursor.map_position.GetColumn().get());
  }
  else
  {
    ImGui::TextUnformatted("Row: N/A, Column: N/A");
  }
}

void MouseTileLabels(const entt::registry& registry,
                     const ViewportCursorInfo& cursor)
{
  const auto& activeLayer = registry.ctx<ActiveLayer>();

  if (activeLayer.entity != entt::null)
  {
    if (registry.all_of<TileLayer>(activeLayer.entity))
    {
      ImGui::Separator();

      const auto global = Sys::GetTile(registry,
                                       activeLayer.entity,
                                       cursor.map_position.GetRow(),
                                       cursor.map_position.GetColumn());
      if (cursor.is_within_map && global)
      {
        ImGui::Text("Global ID: %i", global->get());
      }
      else
      {
        ImGui::TextUnformatted("Global ID: N/A");
      }

      if (global)
      {
        const auto local = Sys::ConvertToLocal(registry, *global);
        if (cursor.is_within_map && local)
        {
          ImGui::Text("Local ID: %i", local->get());
        }
        else
        {
          ImGui::TextUnformatted("Local ID: N/A");
        }
      }
    }
  }
}

}  // namespace

void MapContentOverlay(const entt::registry& registry,
                       const CanvasInfo& canvas,
                       const ViewportCursorInfo& cursor)
{
  const auto* viewport = ImGui::GetWindowViewport();
  const auto position = viewport->WorkPos + ImGui::GetWindowPos() + ImVec2{10, 18};
  ImGui::SetNextWindowPos(position, ImGuiCond_Always);

  ImGui::SetNextWindowBgAlpha(0.35f);
  if (ImGui::Begin("##MapContentOverlay", nullptr, flags))
  {
    MouseCoordinateLabels(cursor);
    MouseRowColumnLabels(cursor);
    MouseTileLabels(registry, cursor);
  }

  ImGui::End();
}

}  // namespace Tactile
