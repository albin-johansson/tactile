#include "render_stamp_preview.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <tactile_def.hpp>

#include "assert.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/tile_position.hpp"
#include "core/systems/map_system.hpp"
#include "editor/gui/texture_utils.hpp"
#include "render_info.hpp"

namespace tactile {
namespace {

constexpr uint32 gPreviewOpacity = 100;  // [0, 255]

struct PreviewInfo final
{
  ImTextureID texture_id{};

  ImVec2 map_pos{};
  ImVec2 grid_size{};
  ImVec2 uv_size{};

  tile_position mouse_pos{};
  tile_position selection_begin;
  tile_position selection_size;
  tile_position offset;
};

void RenderPreviewTile(const PreviewInfo& info,
                       const tile_position& previewTilePos,
                       const ImVec2& tilesetTilePos)
{
  const auto x = static_cast<float>(previewTilePos.col()) * info.grid_size.x;
  const auto y = static_cast<float>(previewTilePos.row()) * info.grid_size.y;
  const auto realPos = info.map_pos + ImVec2{x, y};

  const auto uvMin = tilesetTilePos * info.uv_size;
  const auto uvMax = uvMin + info.uv_size;

  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddImage(info.texture_id,
                     realPos,
                     realPos + info.grid_size,
                     uvMin,
                     uvMax,
                     IM_COL32(0xFF, 0xFF, 0xFF, gPreviewOpacity));
}

void RenderPreviewTiles(const entt::registry& registry, const PreviewInfo& info)
{
  const auto endRow = info.selection_size.row();
  const auto endCol = info.selection_size.col();
  for (auto row = 0; row < endRow; ++row) {
    for (auto col = 0; col < endCol; ++col) {
      const auto position = tile_position{row, col};
      const auto previewTilePos = info.mouse_pos + position - info.offset;

      if (sys::is_position_in_map(registry, previewTilePos)) {
        const auto tilesetTilePos = info.selection_begin + position;
        const auto tilesetTileRow = static_cast<float>(tilesetTilePos.row());
        const auto tilesetTileCol = static_cast<float>(tilesetTilePos.col());
        RenderPreviewTile(info, previewTilePos, {tilesetTileCol, tilesetTileRow});
      }
    }
  }
}

}  // namespace

void RenderStampPreview(const entt::registry& registry,
                        const tile_position& mousePos,
                        const RenderInfo& renderInfo)
{
  const auto& activeTileset = registry.ctx<ActiveTileset>();

  const auto tilesetEntity = activeTileset.entity;
  TACTILE_ASSERT(tilesetEntity != entt::null);

  const auto& selection = registry.get<TilesetSelection>(tilesetEntity);
  if (!selection.region) {
    return;
  }

  const auto& region = selection.region.value();
  const auto& texture = registry.get<Texture>(tilesetEntity);
  const auto& uv = registry.get<UvTileSize>(tilesetEntity);

  PreviewInfo info;
  info.texture_id = ToTextureID(texture.id);

  info.map_pos = renderInfo.origin;
  info.grid_size = renderInfo.grid_size;
  info.uv_size = {uv.width, uv.height};

  info.mouse_pos = mousePos;
  info.selection_begin = region.begin;
  info.selection_size = region.end - region.begin;
  info.offset = info.selection_size / tile_position{2, 2};

  RenderPreviewTiles(registry, info);
}

}  // namespace tactile
