#include "render_tile.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/systems/tileset_system.hpp"
#include "gui/texture_utils.hpp"

namespace Tactile {

void RenderTile(const TileID tile,
                const entt::registry& registry,
                const ImVec2& screenPos,
                const ImVec2& gridSize,
                const float opacity)
{
  const auto tilesetEntity = Sys::FindTileset(registry, tile);
  if (tilesetEntity != entt::null) {
    const auto& texture = registry.get<Texture>(tilesetEntity);

    const auto textureId = ToTextureID(texture.id);
    const auto tileToRender = Sys::GetTileToRender(registry, tilesetEntity, tile);
    const auto& source = Sys::GetSourceRect(registry, tilesetEntity, tileToRender);

    const auto row = static_cast<float>(source.y()) / static_cast<float>(source.height());
    const auto col = static_cast<float>(source.x()) / static_cast<float>(source.width());

    const auto uv = registry.get<UvTileSize>(tilesetEntity);
    const auto uvMin = ImVec2{col * uv.width, row * uv.height};
    const auto uvMax = uvMin + ImVec2{uv.width, uv.height};

    ImGui::GetWindowDrawList()->AddImage(textureId,
                                         screenPos,
                                         screenPos + gridSize,
                                         uvMin,
                                         uvMax,
                                         IM_COL32(255, 255, 255, 255 * opacity));
  }
}

}  // namespace Tactile
