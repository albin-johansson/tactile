// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn_format/gd3_document_converter.hpp"

#include <algorithm>   // replace
#include <cmath>       // sin, cos
#include <filesystem>  // path
#include <format>      // format
#include <numbers>     // pi_v
#include <string>      // string
#include <utility>     // move

#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/document/map_view.hpp"
#include "tactile/base/document/meta_view.hpp"
#include "tactile/base/document/object_view.hpp"
#include "tactile/base/document/tile_view.hpp"
#include "tactile/base/document/tileset_view.hpp"
#include "tactile/base/numeric/literals.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"

namespace tactile::godot {
namespace {

using namespace std::string_literals;

[[nodiscard]]
auto _escape_name(const std::string_view str) -> std::string
{
  std::string copy {str};

  for (const auto bad_char : {'.', ':', '@', '/', '"', '%'}) {
    std::ranges::replace(copy, bad_char, '_');
  }

  return copy;
}

[[nodiscard]]
auto _find_layer_at_global_index(std::vector<Gd3Layer>& layers,
                                 const std::size_t target_index,
                                 std::size_t index = 0) -> Gd3Layer*
{
  for (auto& layer : layers) {
    if (index == target_index) {
      return &layer;
    }

    ++index;

    if (auto* group = std::get_if<Gd3GroupLayer>(&layer.value)) {
      if (auto* found_layer =
              _find_layer_at_global_index(group->layers, target_index, index)) {
        return found_layer;
      }
    }
  }

  return nullptr;
}

[[nodiscard]]
auto _find_tileset_texture_id(const Gd3Tileset& gd_tileset, const TileID first_tile_id)
    -> std::optional<ExtResourceId>
{
  for (const auto& gd_tileset_instance : gd_tileset.instances) {
    if (gd_tileset_instance.first_tile_id == first_tile_id) {
      return gd_tileset_instance.texture_id;
    }
  }

  return std::nullopt;
}

[[nodiscard]]
auto _approximate_ellipse_as_polygon(const Float2 radius, const std::size_t point_count)
    -> std::vector<Float2>
{
  std::vector<Float2> points {};
  points.reserve(point_count);

  const auto n = static_cast<float>(point_count);

  for (auto index = 0_uz; index < point_count; ++index) {
    const auto theta = static_cast<float>(index) / n * std::numbers::pi * 2.0f;

    auto& point = points.emplace_back();
    point.set_x(radius.x() * std::cos(theta));
    point.set_y(radius.y() * std::sin(theta));
  }

  return points;
}

[[nodiscard]]
auto _convert_meta(const IMetaView& meta) -> Gd3Metadata
{
  Gd3Metadata gd_meta {};

  const auto prop_count = meta.property_count();
  for (auto prop_index = 0_uz; prop_index < prop_count; ++prop_index) {
    const auto& [key, value] = meta.get_property(prop_index);
    gd_meta.props.insert_or_assign(key, value);
  }

  return gd_meta;
}

[[nodiscard]]
auto _convert_tile_layer(const ILayerView& layer,
                         const Gd3Tileset& gd_tileset,
                         std::string parent_path) -> Gd3Layer
{
  Gd3Layer gd_layer {};
  gd_layer.name = _escape_name(layer.get_meta().get_name());
  gd_layer.meta = _convert_meta(layer.get_meta());
  gd_layer.parent = std::move(parent_path);
  gd_layer.opacity = layer.get_opacity();
  gd_layer.visible = layer.is_visible();

  auto& gd_tile_layer = gd_layer.value.emplace<Gd3TileLayer>();
  gd_tile_layer.cell_size = layer.get_tile_size();

  const auto extent = layer.get_extent().value();
  for (Extent2D::value_type row = 0; row < extent.rows; ++row) {
    for (Extent2D::value_type col = 0; col < extent.cols; ++col) {
      const Index2D tile_position {.x = col, .y = row};

      const auto tile_id = layer.get_tile(tile_position);
      if (!tile_id.has_value() || tile_id == kEmptyTile) {
        continue;
      }

      const auto position_in_tileset = layer.get_tile_position_in_tileset(*tile_id);
      if (!position_in_tileset.has_value()) {
        continue;
      }

      const auto* gd_tileset_instance = find_tileset_instance(gd_tileset, *tile_id);
      if (!gd_tileset_instance) {
        // TODO error
        continue;
      }

      constexpr std::int32_t tile_offset = 65'536;

      auto& gd_tile = gd_tile_layer.tiles.emplace_back();
      gd_tile.tile_index = *tile_id - gd_tileset_instance->first_tile_id;

      if (gd_tile.tile_index >= gd_tileset_instance->column_count) {
        gd_tile.tile_index = saturate_cast<std::int32_t>(position_in_tileset->x) +
                             saturate_cast<std::int32_t>(position_in_tileset->y) * tile_offset;
      }

      gd_tile.position =
          saturate_cast<std::int32_t>(col) + saturate_cast<std::int32_t>(row) * tile_offset;
      gd_tile.tileset = get_tileset_index(gd_tileset, *tile_id);

      if (layer.is_tile_animated(tile_position)) {
        auto& gd_tile_animation = gd_tile_layer.animations.emplace_back();
        gd_tile_animation.position = tile_position;
        gd_tile_animation.tile_id = *tile_id;
        gd_tile_animation.parent = std::format("{}/{}", gd_layer.parent, gd_layer.name);
      }
    }
  }

  return gd_layer;
}

[[nodiscard]]
auto _convert_object_layer(const ILayerView& layer, std::string parent_path) -> Gd3Layer
{
  const auto object_count = layer.object_count();

  Gd3Layer gd_layer {};
  gd_layer.name = _escape_name(layer.get_meta().get_name());
  gd_layer.meta = _convert_meta(layer.get_meta());
  gd_layer.parent = std::move(parent_path);
  gd_layer.opacity = layer.get_opacity();
  gd_layer.visible = layer.is_visible();

  auto& gd_object_layer = gd_layer.value.emplace<Gd3ObjectLayer>();
  gd_object_layer.objects.reserve(object_count);

  return gd_layer;
}

[[nodiscard]]
auto _convert_group_layer(const ILayerView& layer, std::string parent_path) -> Gd3Layer
{
  const auto layer_count = layer.layer_count();

  Gd3Layer gd_layer {};
  gd_layer.name = _escape_name(layer.get_meta().get_name());
  gd_layer.meta = _convert_meta(layer.get_meta());
  gd_layer.parent = std::move(parent_path);
  gd_layer.opacity = layer.get_opacity();
  gd_layer.visible = layer.is_visible();

  auto& gd_group_layer = gd_layer.value.emplace<Gd3GroupLayer>();
  gd_group_layer.layers.reserve(layer_count);

  return gd_layer;
}

void _convert_tile_animation(const ITileView& tile,
                             Gd3Scene& gd_scene,
                             const ExtResourceId texture_id)
{
  const auto frame_count = tile.animation_frame_count();
  if (frame_count < 1) {
    return;
  }

  const auto& tileset = tile.get_parent_tileset();
  const auto tileset_columns = saturate_cast<TileIndex>(tileset.column_count());

  const auto tile_id = tileset.get_first_tile_id() + tile.get_index();
  const auto tile_size = tileset.get_tile_size();

  Gd3Animation gd_animation {};
  gd_animation.name = std::format("Tile {}", tile_id);
  gd_animation.frames.reserve(frame_count);

  // Godot 3 uses the same speed for all frames, so we pick the speed of the first frame.
  const auto [_, first_frame_duration_ms] = tile.get_animation_frame(0);
  gd_animation.speed = 1'000.0f / static_cast<float>(first_frame_duration_ms.count());

  for (auto frame_index = 0_uz; frame_index < frame_count; ++frame_index) {
    const auto [frame_tile_index, frame_duration] = tile.get_animation_frame(frame_index);
    const auto frame_tile_pos = Index2D::from_1d(frame_tile_index, tileset_columns);

    const Gd3AtlasTexture frame_texture {
      .atlas_id = texture_id,
      .region = Int4 {saturate_cast<Int4::value_type>(frame_tile_pos.x * tile_size.x()),
                      saturate_cast<Int4::value_type>(frame_tile_pos.y * tile_size.y()),
                      tile_size.x(),
                      tile_size.y()},
    };

    const auto atlas_texture_id = gd_scene.next_sub_resource_id++;
    gd_scene.atlas_textures.insert_or_assign(atlas_texture_id, frame_texture);

    gd_animation.frames.push_back(atlas_texture_id);
  }

  if (gd_scene.sprite_frames.animations.empty()) {
    gd_scene.sprite_frames.id = gd_scene.next_sub_resource_id++;
  }

  gd_scene.sprite_frames.animations.push_back(std::move(gd_animation));
}

}  // namespace

Gd3DocumentConverter::Gd3DocumentConverter(IRuntime* runtime, SaveFormatWriteOptions options)
  : m_runtime {runtime},
    m_options {std::move(options)},
    m_map {}
{}

auto Gd3DocumentConverter::visit(const IComponentView&) -> std::expected<void, std::error_code>
{
  return {};
}

auto Gd3DocumentConverter::visit(const IMapView& map) -> std::expected<void, std::error_code>
{
  m_map.scene.next_ext_resource_id = 1;
  m_map.scene.next_sub_resource_id = 1;

  m_map.tileset.scene.next_ext_resource_id = 1;
  m_map.tileset.scene.next_sub_resource_id = 1;

  m_map.tileset_id = m_map.scene.next_ext_resource_id++;
  m_map.scene.root_meta = _convert_meta(map.get_meta());

  m_map.scene.ext_resources[m_map.tileset_id] = Gd3ExtResource {
    .path = "res://tileset.tres",  // FIXME
    .type = "TileSet",
  };

  return {};
}

auto Gd3DocumentConverter::visit(const ILayerView& layer)
    -> std::expected<void, std::error_code>
{
  const auto* parent_layer = layer.get_parent_layer();

  auto* gd_parent_layer =
      parent_layer != nullptr
          ? _find_layer_at_global_index(m_map.layers, parent_layer->get_global_index())
          : nullptr;

  const auto parent_path =
      gd_parent_layer != nullptr
          ? std::format("{}/{}", gd_parent_layer->parent, gd_parent_layer->name)
          : "."s;

  Gd3Layer gd_layer {};
  switch (layer.get_type()) {
    case LayerType::kTileLayer: {
      gd_layer = _convert_tile_layer(layer, m_map.tileset, parent_path);
      break;
    }

    case LayerType::kObjectLayer: {
      gd_layer = _convert_object_layer(layer, parent_path);
      break;
    }

    case LayerType::kGroupLayer: {
      gd_layer = _convert_group_layer(layer, parent_path);
      break;
    }
  }

  if (gd_parent_layer != nullptr) {
    std::get<Gd3GroupLayer>(gd_parent_layer->value).layers.push_back(std::move(gd_layer));
  }
  else {
    m_map.layers.push_back(std::move(gd_layer));
  }

  return {};
}

auto Gd3DocumentConverter::visit(const IObjectView& object)
    -> std::expected<void, std::error_code>
{
  const auto* parent_layer = object.get_parent_layer();
  if (!parent_layer) {
    // TODO figure out how to handle objects in tiles
    return {};
  }

  auto* gd_parent_layer =
      _find_layer_at_global_index(m_map.layers, parent_layer->get_global_index());
  if (!gd_parent_layer) {
    return std::unexpected {std::make_error_code(std::errc::invalid_argument)};
  }

  auto object_name = std::format("Object {}", object.get_id());
  if (const auto real_object_name = object.get_meta().get_name(); !real_object_name.empty()) {
    object_name += std::format(" ('{}')", _escape_name(real_object_name));
  }

  Gd3Object gd_object {};
  gd_object.name = std::move(object_name);
  gd_object.parent = gd_parent_layer->parent;
  gd_object.meta = _convert_meta(object.get_meta());
  gd_object.visible = object.is_visible();

  switch (object.get_type()) {
    case ObjectType::kPoint: {
      gd_object.position = object.get_position();
      gd_object.value.emplace<Gd3Point>();
      break;
    }
    case ObjectType::kRect: {
      gd_object.position = object.get_position() + object.get_size() * 0.5f;

      const auto shape_id = m_map.scene.next_sub_resource_id++;

      auto& gd_rect = gd_object.value.emplace<Gd3Rect>();
      gd_rect.shape_id = shape_id;

      m_map.scene.rect_shapes[shape_id] = Gd3RectShape {
        .extents = object.get_size() * 0.5f,
      };

      break;
    }
    case ObjectType::kEllipse: {
      gd_object.position = object.get_position() + object.get_size() * 0.5f;

      auto& gd_polygon = gd_object.value.emplace<Gd3Polygon>();

      const auto radius = object.get_size() * 0.5f;
      constexpr std::size_t point_count = 32;  // FIXME
      gd_polygon.points = _approximate_ellipse_as_polygon(radius, point_count);

      break;
    }
  }

  auto& gd_object_layer = std::get<Gd3ObjectLayer>(gd_parent_layer->value);
  gd_object_layer.objects.push_back(std::move(gd_object));

  return {};
}

auto Gd3DocumentConverter::visit(const ITilesetView& tileset)
    -> std::expected<void, std::error_code>
{
  const auto& source_image_path = tileset.get_image_path();

  const auto texture_id = m_map.tileset.scene.next_ext_resource_id++;
  const auto texture_image_name = source_image_path.filename().string();

  m_map.tileset.scene.ext_resources[texture_id] = Gd3ExtResource {
    .path = std::format("res://{}", texture_image_name),
    .type = "Texture",
  };
  m_map.tileset.texture_paths.push_back(source_image_path);

  auto& gd_tileset_instance = m_map.tileset.instances.emplace_back();
  gd_tileset_instance.name = _escape_name(tileset.get_meta().get_name());
  gd_tileset_instance.texture_id = texture_id;
  gd_tileset_instance.first_tile_id = tileset.get_first_tile_id();
  gd_tileset_instance.tile_count = tileset.tile_count();
  gd_tileset_instance.column_count = tileset.column_count();
  gd_tileset_instance.tile_size = tileset.get_tile_size();
  gd_tileset_instance.image_size = tileset.get_image_size();

  return {};
}

auto Gd3DocumentConverter::visit(const ITileView& tile) -> std::expected<void, std::error_code>
{
  const auto& tileset = tile.get_parent_tileset();

  const auto texture_id =
      _find_tileset_texture_id(m_map.tileset, tileset.get_first_tile_id()).value();
  _convert_tile_animation(tile, m_map.tileset.scene, texture_id);

  return {};
}

auto Gd3DocumentConverter::get_map() const -> const Gd3Map&
{
  return m_map;
}

}  // namespace tactile::godot
