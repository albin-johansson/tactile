// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/render/orthogonal_renderer.hpp"

#include <algorithm>  // min

#include "tactile/base/container/lookup.hpp"
#include "tactile/base/meta/color.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/tile/tileset.hpp"
#include "tactile/core/ui/canvas_renderer.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/render/primitives.hpp"

namespace tactile::core::ui {
namespace {

void _render_tile(const CanvasRenderer& canvas_renderer,
                  const Index2D& position_in_world,
                  const Index2D& position_in_tileset,
                  void* texture_handle,
                  const Float2& uv_tile_size)
{
  const auto position_in_texture = to_float2(position_in_tileset) * uv_tile_size;

  const auto world_pos = canvas_renderer.to_screen_pos(position_in_world);
  const auto world_size = canvas_renderer.get_canvas_tile_size();

  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddImage(texture_handle,
                      to_imvec2(world_pos),
                      to_imvec2(world_pos + world_size),
                      to_imvec2(position_in_texture),
                      to_imvec2(position_in_texture + uv_tile_size));
}

void _render_tile_layer(const CanvasRenderer& canvas_renderer,
                        const Registry& registry,
                        const EntityID layer_id)
{
  const auto& render_bounds = canvas_renderer.get_render_bounds();
  const auto& tile_cache = registry.get<CTileCache>();

  each_layer_tile(
      registry,
      layer_id,
      render_bounds.begin,
      render_bounds.end,
      [&](const Index2D& position_in_world, const TileID tile_id) {
        if (tile_id == kEmptyTile) {
          return;
        }

        const auto tileset_id = lookup_in(tile_cache.tileset_mapping, tile_id);

        const auto& texture = registry.get<CTexture>(tileset_id);
        const auto& tileset = registry.get<CTileset>(tileset_id);
        const auto& tileset_instance = registry.get<CTilesetInstance>(tileset_id);

        const TileIndex tile_index {tile_id - tileset_instance.tile_range.first_id};
        const auto apparent_tile_index = get_tile_appearance(registry, tileset_id, tile_index);

        const auto position_in_tileset =
            Index2D::from_1d(static_cast<Extent2D::value_type>(apparent_tile_index),
                             tileset.extent.cols);

        _render_tile(canvas_renderer,
                     position_in_world,
                     position_in_tileset,
                     texture.raw_handle,
                     tileset.uv_tile_size);
      });
}

void _render_object(const CanvasRenderer& canvas_renderer,
                    const Registry& registry,
                    const EntityID object_id)
{
  const auto& object = registry.get<CObject>(object_id);

  if (!object.is_visible) {
    return;
  }

  // TODO cull objects outside of render bounds

  const auto canvas_scale = canvas_renderer.get_scale();
  const auto scaled_pos = object.position * canvas_scale;
  const auto scaled_size = object.size * canvas_scale;
  const auto screen_pos = canvas_renderer.to_screen_pos(scaled_pos);

  constexpr auto line_color = kColorYellow;
  constexpr auto line_thickness = 2.0f;

  switch (object.type) {
    case ObjectType::kPoint: {
      const auto radius = std::min(canvas_renderer.get_canvas_tile_size().x() * 0.2f, 6.0f);
      draw_circle_shadowed(screen_pos, radius, line_color, line_thickness);
      break;
    }
    case ObjectType::kRect: {
      draw_rect_shadowed(screen_pos, scaled_size, line_color, line_thickness);
      break;
    }
    case ObjectType::kEllipse: {
      const auto radius = scaled_size * 0.5f;
      draw_ellipse_shadowed(screen_pos + radius, radius, line_color, line_thickness);
      break;
    }
  }
}

void _render_object_layer(const CanvasRenderer& canvas_renderer,
                          const Registry& registry,
                          const EntityID layer_id)
{
  const auto& object_layer = registry.get<CObjectLayer>(layer_id);
  for (const auto object_id : object_layer.objects) {
    _render_object(canvas_renderer, registry, object_id);
  }
}

void _render_layer(const CanvasRenderer& canvas_renderer,
                   const Registry& registry,
                   const EntityID layer_id)
{
  if (const auto& layer = registry.get<CLayer>(layer_id); !layer.visible) {
    return;
  }

  if (is_tile_layer(registry, layer_id)) {
    _render_tile_layer(canvas_renderer, registry, layer_id);
  }
  else if (is_object_layer(registry, layer_id)) {
    _render_object_layer(canvas_renderer, registry, layer_id);
  }
  else if (is_group_layer(registry, layer_id)) {
    const auto& group_layer = registry.get<CGroupLayer>(layer_id);
    for (const auto sublayer_id : group_layer.layers) {
      _render_layer(canvas_renderer, registry, sublayer_id);
    }
  }
}

}  // namespace

void render_orthogonal_map(const CanvasRenderer& canvas_renderer,
                           const Registry& registry,
                           const EntityID map_id)
{
  TACTILE_ASSERT(is_map(registry, map_id));

  constexpr UColor bg_color {50, 50, 50, 255};
  constexpr UColor border_color {255, 0, 0, 255};
  constexpr UColor grid_color {0, 0, 0, 50};

  canvas_renderer.clear_canvas(bg_color);

  const auto& map = registry.get<CMap>(map_id);
  const auto& root_layer = registry.get<CGroupLayer>(map.root_layer);

  for (const auto layer_id : root_layer.layers) {
    _render_layer(canvas_renderer, registry, layer_id);
  }

  canvas_renderer.draw_orthogonal_grid(grid_color);

  const auto tile_size = canvas_renderer.get_canvas_tile_size();
  const Float2 map_size {
    static_cast<float>(map.extent.cols) * tile_size.x(),
    static_cast<float>(map.extent.rows) * tile_size.y(),
  };

  draw_rect(canvas_renderer.to_screen_pos(Float2 {0, 0}), map_size, border_color, 2.0f);
}

}  // namespace tactile::core::ui
