#include "render_tile.hpp"

#include <imgui.h>

#include "core/components/texture.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/gui/texture_utils.hpp"
#include "graphics.hpp"

namespace Tactile {

void RenderTile(Graphics& graphics,
                const entt::registry& registry,
                const TileID tile,
                const int32 row,
                const int32 column)
{
  const auto tilesetEntity = Sys::FindTilesetWithTile(registry, tile);
  if (tilesetEntity != entt::null) {
    const auto& texture = registry.get<Texture>(tilesetEntity);
    const auto& uvTileSize = registry.get<UvTileSize>(tilesetEntity);

    const auto tileToRender = Sys::GetTileToRender(registry, tilesetEntity, tile);
    const auto& sourceRect = Sys::GetSourceRect(registry, tilesetEntity, tileToRender);

    const ImVec4 source{static_cast<float>(sourceRect.x()),
                        static_cast<float>(sourceRect.y()),
                        static_cast<float>(sourceRect.width()),
                        static_cast<float>(sourceRect.height())};
    const ImVec2 uv{uvTileSize.width, uvTileSize.height};

    const auto position = graphics.FromMatrixToAbsolute(row, column);
    graphics.RenderTranslatedImage(texture.id, source, position, uv);
  }
}

}  // namespace Tactile
