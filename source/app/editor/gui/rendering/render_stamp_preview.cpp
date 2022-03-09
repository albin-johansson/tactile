/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "render_stamp_preview.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/systems/map_system.hpp"
#include "core/tile_position.hpp"
#include "editor/gui/textures.hpp"
#include "misc/assert.hpp"
#include "render_info.hpp"
#include "tactile.hpp"

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
                        const render_info& renderInfo)
{
  const auto& activeTileset = registry.ctx<comp::active_tileset>();

  const auto tilesetEntity = activeTileset.entity;
  TACTILE_ASSERT(tilesetEntity != entt::null);

  const auto& selection = registry.get<comp::tileset_selection>(tilesetEntity);
  if (!selection.region) {
    return;
  }

  const auto& region = selection.region.value();
  const auto& texture = registry.get<comp::texture>(tilesetEntity);
  const auto& uv = registry.get<comp::uv_tile_size>(tilesetEntity);

  PreviewInfo info;
  info.texture_id = to_texture_id(texture.id);

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
