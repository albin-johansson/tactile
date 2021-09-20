#include "render_map.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "io/preferences.hpp"
#include "render_bounds.hpp"
#include "render_info.hpp"
#include "render_object_layer.hpp"
#include "render_tile_layer.hpp"

namespace Tactile {
namespace {

constexpr auto out_border_color = IM_COL32(0xFF, 0xFF, 0xFF, 0xFF);
constexpr auto in_border_color = IM_COL32(0xFF, 0xFF, 0xFF, 20);

void RenderOutline(const RenderInfo& info)
{
  const auto width = info.col_count * info.grid_size.x;
  const auto height = info.row_count * info.grid_size.y;
  ImGui::GetWindowDrawList()->AddRect(info.map_position,
                                      info.map_position + ImVec2{width, height},
                                      out_border_color);
}

void RenderGrid(const RenderInfo& info)
{
  auto* drawList = ImGui::GetWindowDrawList();

  const auto& bounds = info.bounds;
  const auto endRow = bounds.end.GetRow();
  const auto endCol = bounds.end.GetColumn();
  for (auto row = bounds.begin.GetRow(); row < endRow; ++row) {
    for (auto col = bounds.begin.GetColumn(); col < endCol; ++col) {
      const ImVec2 pos = {
          info.map_position.x + (info.grid_size.x * static_cast<float>(col)),
          info.map_position.y + (info.grid_size.y * static_cast<float>(row))};
      drawList->AddRect(pos, pos + info.grid_size, in_border_color);
    }
  }
}

void RenderLayer(const entt::registry& registry,
                 const entt::entity layerEntity,
                 const Layer& layer,
                 const RenderInfo& info,
                 const float parentOpacity)
{
  if (layer.type == LayerType::TileLayer) {
    RenderTileLayer(registry, layerEntity, info, parentOpacity);
  }
  else if (layer.type == LayerType::ObjectLayer) {
    RenderObjectLayer(registry, layerEntity, info, parentOpacity);
  }
}

}  // namespace

void RenderMap(const entt::registry& registry, const RenderInfo& info)
{
  for (auto&& [entity, layer] : registry.view<Layer>().each()) {
    const auto& parent = registry.get<Parent>(entity);
    const auto* parentLayer =
        (parent.entity != entt::null) ? registry.try_get<Layer>(parent.entity) : nullptr;

    const auto parentOpacity = parentLayer ? parentLayer->opacity : 1.0f;

    if (layer.visible) {
      if (!parentLayer || parentLayer->visible) {
        RenderLayer(registry, entity, layer, info, layer.opacity * parentOpacity);
      }
    }
  }

  if (Prefs::GetShowGrid()) {
    RenderGrid(info);
  }

  RenderOutline(info);
}

}  // namespace Tactile
