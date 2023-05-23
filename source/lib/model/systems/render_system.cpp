/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "render_system.hpp"

#include <algorithm>  // min

#include <imgui.h>
#include <imgui_internal.h>

#include "common/debug/assert.hpp"
#include "components/context.hpp"
#include "components/texture.hpp"
#include "core/layer.hpp"
#include "core/object.hpp"
#include "model/settings.hpp"
#include "model/systems/group_layer_system.hpp"
#include "model/systems/layer_system.hpp"
#include "model/systems/tileset_system.hpp"
#include "ui/conversions.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"

namespace tactile::sys {
namespace {

inline constexpr Color kActiveObjectColor {0xFF, 0xFF, 0x00, 0xFF};

void _render_point_object(const Model& model,
                          const ui::CanvasInfo& canvas,
                          const Entity object_entity,
                          const ImVec2& rendered_position,
                          const Color& color)
{
  const auto& object = model.get<Object>(object_entity);
  const auto& object_context = model.get<Context>(object_entity);

  const auto translated_position = ui::translate_pos(canvas, rendered_position);
  const auto point_radius = std::min(canvas.graphical_tile_size.x / 4.0f, 6.0f);

  if (canvas.contains(translated_position)) {
    ui::draw_shadowed_circle(translated_position, point_radius, color, 2.0f);

    if (!object_context.name.empty()) {
      const auto text_size = ImGui::CalcTextSize(object_context.name.c_str());

      if (text_size.x <= canvas.graphical_tile_size.x) {
        ImVec2 text_pos;
        text_pos.x = object.position.x - (text_size.x / 2.0f);
        text_pos.y = object.position.y + point_radius + 4.0f;

        ui::render_text(object_context.name.c_str(),
                        ui::translate_pos(canvas, text_pos),
                        kWhite);
      }
    }
  }
}

void _render_rectangle_object(const Model& model,
                              const ui::CanvasInfo& canvas,
                              const Entity object_entity,
                              const ImVec2& rendered_position,
                              const Color& color)
{
  const auto& object = model.get<Object>(object_entity);
  const auto& object_context = model.get<Context>(object_entity);

  const auto translated_position = ui::translate_pos(canvas, rendered_position);
  const auto rendered_size = as_imvec2(object.size) * canvas.tile_ratio;
  const ImVec4 rect_bounds {translated_position.x,
                            translated_position.y,
                            rendered_size.x,
                            rendered_size.y};

  if (canvas.intersects(rect_bounds)) {
    ui::draw_shadowed_rect(translated_position, rendered_size, color, 2.0f);

    if (!object_context.name.empty()) {
      const auto text_size = ImGui::CalcTextSize(object_context.name.c_str());

      if (text_size.x <= rendered_size.x) {
        ImVec2 text_offset;
        text_offset.x = (rendered_size.x - text_size.x) / 2.0f;
        text_offset.y = rendered_size.y + 4.0f;

        const auto text_pos = ui::translate_pos(canvas, rendered_position + text_offset);
        ui::render_text(object_context.name.c_str(), text_pos, kWhite);
      }
    }
  }
}

void _render_ellipse_object(const Model& model,
                            const ui::CanvasInfo& canvas,
                            const Entity object_entity,
                            const ImVec2& rendered_position,
                            const Color& color)
{
  const auto& object = model.get<Object>(object_entity);
  const auto& object_context = model.get<Context>(object_entity);

  const auto rendered_size = as_imvec2(object.size) * canvas.tile_ratio;

  const auto ellipse_radius = ImVec2 {0.5f, 0.5f} * rendered_size;
  const auto ellipse_center = rendered_position + ellipse_radius;

  ui::draw_shadowed_ellipse(ui::translate_pos(canvas, ellipse_center),
                            ellipse_radius,
                            color,
                            2.0f);

  if (!object_context.name.empty()) {
    const auto text_size = ImGui::CalcTextSize(object_context.name.c_str());

    if (text_size.x <= ellipse_radius.x) {
      ImVec2 text_pos;
      text_pos.x = ellipse_center.x - (text_size.x / 2.0f);
      text_pos.y = ellipse_center.y + (text_size.y / 2.0f) + (ellipse_radius.y);

      ui::render_text(object_context.name.c_str(),
                      ui::translate_pos(canvas, text_pos),
                      kWhite);
    }
  }
}

void _render_layers(const Model& model, const ui::CanvasInfo& canvas, const Map& map)
{
  // TODO performance: include parent layer entity in function object signature
  sys::recurse_layers(model, map.root_layer, [&](const Entity layer_entity) {
    const auto parent_layer_entity =
        sys::get_parent_layer(model, map.root_layer, layer_entity);
    render_layer(model, canvas, map, parent_layer_entity, layer_entity);
  });
}

void _render_active_object_highlight(const Model& model,
                                     const ui::CanvasInfo& canvas,
                                     const Map& map)
{
  if (map.active_layer != kNullEntity && model.has<ObjectLayer>(map.active_layer)) {
    const auto& object_layer = model.get<ObjectLayer>(map.active_layer);
    if (object_layer.active_object != kNullEntity) {
      render_object(model, canvas, object_layer.active_object, kActiveObjectColor);
    }
  }
}

}  // namespace

void render_map(const Model& model, const ui::CanvasInfo& canvas, const Map& map)
{
  const auto& settings = model.get<Settings>();

  _render_layers(model, canvas, map);
  _render_active_object_highlight(model, canvas, map);

  if (settings.test_flag(SETTINGS_SHOW_GRID_BIT)) {
    ui::render_infinite_grid(canvas, settings.get_grid_color());
  }

  ui::render_outline(canvas, to_color(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]));
}

void render_tileset(const Model& model,
                    const ui::CanvasInfo& canvas,
                    const Tileset& tileset)
{
  const auto& settings = model.get<Settings>();
  const auto& texture = model.get<Texture>(tileset.texture);

  const auto rendered_position = ui::translate_pos(canvas, ImVec2 {0, 0});
  const auto rendered_size = as_imvec2(texture.size) * canvas.tile_ratio;

  ui::render_image(texture.handle, rendered_position, rendered_size);

  if (settings.test_flag(SETTINGS_SHOW_GRID_BIT)) {
    ui::render_infinite_grid(canvas, settings.get_grid_color());
  }

  ui::render_outline(canvas, to_color(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]));
}

void render_layer(const Model& model,
                  const ui::CanvasInfo& canvas,
                  const Map& map,
                  const Entity parent_layer_entity,
                  const Entity layer_entity)
{
  const auto& layer = model.get<Layer>(layer_entity);
  const Layer* parent_layer = (parent_layer_entity != kNullEntity)
                                  ? model.try_get<Layer>(parent_layer_entity)
                                  : nullptr;

  // Return early if the layer or parent layer are invisible
  if (!layer.visible || (parent_layer != nullptr && !parent_layer->visible)) {
    return;
  }

  const auto& settings = model.get<Settings>();

  const auto parent_opacity = (parent_layer != nullptr) ? parent_layer->opacity : 1.0f;
  const auto layer_opacity = settings.test_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT)
                                 ? (map.active_layer == layer_entity ? 1.0f : 0.5f)
                                 : parent_opacity * layer.opacity;

  if (layer.type == LayerType::TileLayer) {
    render_tile_layer(model, canvas, map, layer_entity, layer_opacity);
  }
  else if (layer.type == LayerType::ObjectLayer) {
    render_object_layer(model, canvas, layer_entity, layer_opacity);
  }
}

void render_tile_layer(const Model& model,
                       const ui::CanvasInfo& canvas,
                       const Map& map,
                       const Entity tile_layer_entity,
                       const float opacity)
{
  const auto& tile_layer = model.get<TileLayer>(tile_layer_entity);

  const auto begin_row = canvas.bounds.begin.row();
  const auto begin_col = canvas.bounds.begin.col();
  const auto end_row = canvas.bounds.end.row();
  const auto end_col = canvas.bounds.end.col();

  for (auto row = begin_row; row < end_row; ++row) {
    for (auto col = begin_col; col < end_col; ++col) {
      const TilePos tile_pos {row, col};

      const auto tile_id = tile_layer.tile_at(tile_pos);
      if (tile_id.has_value() && tile_id != kEmptyTile) {
        render_tile(model, canvas, map, *tile_id, tile_pos, opacity);
      }
    }
  }
}

void render_tile(const Model& model,
                 const ui::CanvasInfo& canvas,
                 const Map& map,
                 const TileID tile_id,
                 const TilePos& pos,
                 const float opacity)
{
  TACTILE_ASSERT(tile_id != kEmptyTile);
  const auto attached_tileset_entity = sys::find_tileset_with_tile(model, map, tile_id);

  if (attached_tileset_entity != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);

    const auto& tileset = model.get<Tileset>(attached_tileset.tileset);
    const auto& texture = model.get<Texture>(tileset.texture);

    const auto logical_tile_index = attached_tileset.to_tile_index(tile_id).value();
    const auto rendered_tile_index =
        sys::get_tile_appearance(model, attached_tileset.tileset, logical_tile_index);

    const auto pos_in_tileset =
        TilePos::from_index(rendered_tile_index, tileset.column_count);

    ui::render_tile_image(canvas,
                          texture.handle,
                          tileset.uv_size,
                          pos_in_tileset,
                          ui::translate_tile_pos(canvas, pos),
                          opacity);
  }
}

void render_object_layer(const Model& model,
                         const ui::CanvasInfo& canvas,
                         const Entity object_layer_entity,
                         const float opacity)
{
  const auto& object_layer = model.get<ObjectLayer>(object_layer_entity);

  const Color object_color {0xFF, 0, 0, opacity_cast(opacity)};

  for (const auto object_entity: object_layer.objects) {
    render_object(model, canvas, object_entity, object_color);
  }
}

void render_object(const Model& model,
                   const ui::CanvasInfo& canvas,
                   const Entity object_entity,
                   const Color& color)
{
  const auto& object = model.get<Object>(object_entity);
  const auto rendered_position = as_imvec2(object.position) * canvas.tile_ratio;

  switch (object.type) {
    case ObjectType::Point:
      _render_point_object(model, canvas, object_entity, rendered_position, color);
      break;

    case ObjectType::Rect:
      _render_rectangle_object(model, canvas, object_entity, rendered_position, color);
      break;

    case ObjectType::Ellipse:
      _render_ellipse_object(model, canvas, object_entity, rendered_position, color);
      break;
  }
}

}  // namespace tactile::sys
