#include "render_tile.hpp"

#include <imgui.h>

#include "core/components/texture.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/gui/texture_utils.hpp"
#include "graphics.hpp"

namespace tactile {

void RenderTile(graphics_ctx& graphics,
                const entt::registry& registry,
                const tile_id tile,
                const int32 row,
                const int32 column)
{
  const auto& context = registry.ctx<comp::tileset_context>();
  const auto tilesetEntity = context.tile_to_tileset.at(tile);
  if (tilesetEntity != entt::null) {
    const auto& texture = registry.get<comp::texture>(tilesetEntity);
    const auto& uvTileSize = registry.get<comp::uv_tile_size>(tilesetEntity);

    const auto tileToRender = sys::get_tile_to_render(registry, tilesetEntity, tile);
    const auto& sourceRect = sys::get_source_rect(registry, tilesetEntity, tileToRender);

    const ImVec4 source{static_cast<float>(sourceRect.x()),
                        static_cast<float>(sourceRect.y()),
                        static_cast<float>(sourceRect.width()),
                        static_cast<float>(sourceRect.height())};
    const ImVec2 uv{uvTileSize.width, uvTileSize.height};

    const auto position = graphics.from_matrix_to_absolute(row, column);
    graphics.render_translated_image(texture.id, source, position, uv);
  }
}

}  // namespace tactile
