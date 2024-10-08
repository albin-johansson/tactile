// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn/gd3_document_converter.hpp"

#include <algorithm>   // replace
#include <cassert>     // assert
#include <cmath>       // sin, cos
#include <filesystem>  // path
#include <format>      // format
#include <numbers>     // pi_v
#include <stdexcept>   // runtime_error
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

namespace tactile::godot_tscn {
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
auto _approximate_ellipse_as_polygon(const Float2 radius, const std::size_t point_count)
    -> std::vector<Float2>
{
  std::vector<Float2> points {};
  points.reserve(point_count);

  const auto n = static_cast<float>(point_count);

  for (auto index = 0_uz; index < point_count; ++index) {
    const auto theta = static_cast<float>(index) / n * std::numbers::pi_v<float> * 2.0f;

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

void _convert_common_layer_data(const ILayerView& layer,
                                Gd3Layer& gd_layer,
                                std::string parent_path)
{
  gd_layer.name = _escape_name(layer.get_meta().get_name());
  gd_layer.meta = _convert_meta(layer.get_meta());
  gd_layer.parent = std::move(parent_path);
  gd_layer.opacity = layer.get_opacity();
  gd_layer.visible = layer.is_visible();
}

[[nodiscard]]
auto _convert_tile(const ILayerView& layer,
                   const Gd3Tileset& gd_tileset,
                   const Index2D& tile_pos,
                   const TileID tile_id) -> Gd3EncodedTile
{
  const auto& [gd_tile_atlas_index, gd_tile_atlas] = find_tile_atlas(gd_tileset, tile_id);
  if (!gd_tile_atlas) {
    throw std::runtime_error {"could not find tile atlas"};
  }

  constexpr std::int32_t tile_offset = 65'536;

  Gd3EncodedTile gd_tile {};
  gd_tile.tile_index = tile_id - gd_tile_atlas->first_tile_id;

  if (gd_tile.tile_index >= gd_tile_atlas->column_count) {
    const auto position_in_tileset = layer.get_tile_position_in_tileset(tile_id).value();
    gd_tile.tile_index = saturate_cast<std::int32_t>(position_in_tileset.x) +
                         saturate_cast<std::int32_t>(position_in_tileset.y) * tile_offset;
  }

  gd_tile.position = saturate_cast<std::int32_t>(tile_pos.x) +
                     saturate_cast<std::int32_t>(tile_pos.y) * tile_offset;
  gd_tile.tile_atlas = gd_tile_atlas_index;

  return gd_tile;
}

[[nodiscard]]
auto _convert_tile_layer(const ILayerView& layer,
                         const Int2 tile_size,
                         const Gd3Tileset& gd_tileset,
                         std::string parent_path) -> Gd3Layer
{
  Gd3Layer gd_layer {};
  _convert_common_layer_data(layer, gd_layer, std::move(parent_path));

  auto& gd_tile_layer = gd_layer.value.emplace<Gd3TileLayer>();
  gd_tile_layer.cell_size = tile_size;

  const auto extent = layer.get_extent().value();
  for (Extent2D::value_type row = 0; row < extent.rows; ++row) {
    for (Extent2D::value_type col = 0; col < extent.cols; ++col) {
      const Index2D tile_pos {.x = col, .y = row};

      const auto tile_id = layer.get_tile(tile_pos).value();
      if (tile_id == kEmptyTile) {
        continue;
      }

      const auto gd_encoded_tile = _convert_tile(layer, gd_tileset, tile_pos, tile_id);
      gd_tile_layer.tiles.push_back(gd_encoded_tile);

      if (layer.is_tile_animated(tile_pos)) {
        auto& gd_tile_animation = gd_tile_layer.animations.emplace_back();
        gd_tile_animation.position = tile_pos;
        gd_tile_animation.tile_id = tile_id;
        gd_tile_animation.parent = std::format("{}/{}", gd_layer.parent, gd_layer.name);
      }
    }
  }

  return gd_layer;
}

[[nodiscard]]
auto _prepare_object_layer(const ILayerView& layer, std::string parent_path) -> Gd3Layer
{
  Gd3Layer gd_layer {};
  _convert_common_layer_data(layer, gd_layer, std::move(parent_path));

  auto& gd_object_layer = gd_layer.value.emplace<Gd3ObjectLayer>();
  gd_object_layer.objects.reserve(layer.object_count());

  return gd_layer;
}

[[nodiscard]]
auto _prepare_group_layer(const ILayerView& layer, std::string parent_path) -> Gd3Layer
{
  Gd3Layer gd_layer {};
  _convert_common_layer_data(layer, gd_layer, std::move(parent_path));

  auto& gd_group_layer = gd_layer.value.emplace<Gd3GroupLayer>();
  gd_group_layer.layers.reserve(layer.layer_count());

  return gd_layer;
}

void _convert_tile_animation(const ITileView& tile,
                             Gd3Map& gd_map,
                             const ExtResourceId texture_id)
{
  const auto frame_count = tile.animation_frame_count();
  assert(frame_count > 0);

  const auto& tileset = tile.get_parent_tileset();
  const auto tileset_columns = saturate_cast<TileIndex>(tileset.column_count());

  const auto tile_id = tileset.get_first_tile_id() + tile.get_index();
  const auto tile_size = tileset.get_tile_size();

  Gd3Animation gd_animation {};
  gd_animation.name = std::format("Tile {}", tile_id);
  gd_animation.frames.reserve(frame_count);

  const auto [_, first_frame_duration_ms] = tile.get_animation_frame(0);
  gd_animation.speed = 1'000.0f / static_cast<float>(first_frame_duration_ms.count());

  for (auto frame_index = 0_uz; frame_index < frame_count; ++frame_index) {
    const auto [frame_tile_index, frame_duration] = tile.get_animation_frame(frame_index);
    const auto frame_tile_pos = Index2D::from_1d(frame_tile_index, tileset_columns);

    const Gd3AtlasTexture frame_texture {
      .atlas_id = texture_id,
      .region =
          Int4 {
            saturate_cast<Int4::value_type>(frame_tile_pos.x *
                                            saturate_cast<Index2D::value_type>(tile_size.x())),
            saturate_cast<Int4::value_type>(frame_tile_pos.y *
                                            saturate_cast<Index2D::value_type>(tile_size.y())),
            tile_size.x(),
            tile_size.y(),
          },
    };

    const auto atlas_texture_id = gd_map.resources.next_sub_resource_id++;
    gd_map.atlas_textures.insert_or_assign(atlas_texture_id, frame_texture);

    gd_animation.frames.push_back(atlas_texture_id);
  }

  if (gd_map.sprite_frames.animations.empty()) {
    gd_map.sprite_frames.id = gd_map.resources.next_sub_resource_id++;
  }

  gd_map.sprite_frames.animations.push_back(std::move(gd_animation));
}

}  // namespace

Gd3DocumentConverter::Gd3DocumentConverter(SaveFormatWriteOptions options)
  : m_options {std::move(options)},
    m_map {}
{}

void Gd3DocumentConverter::set_ellipse_polygon_vertices(const std::size_t count)
{
  m_ellipse_polygon_vertices = count;
}

auto Gd3DocumentConverter::visit(const IComponentView&) -> std::expected<void, std::error_code>
{
  return {};
}

auto Gd3DocumentConverter::visit(const IMapView& map) -> std::expected<void, std::error_code>
{
  m_map.resources.next_ext_resource_id = 1;
  m_map.resources.next_sub_resource_id = 1;

  m_map.tileset.id = m_map.resources.next_sub_resource_id++;
  m_map.sprite_frames.id = m_map.resources.next_sub_resource_id++;

  m_map.tile_size = map.get_tile_size();
  m_map.meta = _convert_meta(map.get_meta());

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
      gd_layer = _convert_tile_layer(layer, m_map.tile_size, m_map.tileset, parent_path);
      break;
    }

    case LayerType::kObjectLayer: {
      gd_layer = _prepare_object_layer(layer, parent_path);
      break;
    }

    case LayerType::kGroupLayer: {
      gd_layer = _prepare_group_layer(layer, parent_path);
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
    // TODO
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
  gd_object.parent = std::format("{}/{}", gd_parent_layer->parent, gd_parent_layer->name);
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

      const auto shape_id = m_map.resources.next_sub_resource_id++;

      auto& gd_rect = gd_object.value.emplace<Gd3Rect>();
      gd_rect.shape_id = shape_id;

      m_map.rect_shapes[shape_id] = Gd3RectShape {
        .extents = object.get_size() * 0.5f,
      };

      break;
    }
    case ObjectType::kEllipse: {
      gd_object.position = object.get_position() + object.get_size() * 0.5f;

      auto& gd_polygon = gd_object.value.emplace<Gd3Polygon>();

      const auto radius = object.get_size() * 0.5f;
      gd_polygon.points = _approximate_ellipse_as_polygon(radius, m_ellipse_polygon_vertices);

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

  const auto texture_id = m_map.resources.next_ext_resource_id++;
  const auto texture_image_name = source_image_path.filename().string();

  const Gd3ExtResource texture_resource {
    .path = std::format("res://{}", texture_image_name),
    .type = "Texture",
  };

  m_map.resources.ext_resources[texture_id] = texture_resource;
  m_map.tileset_texture_ids[tileset.get_first_tile_id()] = texture_id;

  auto& gd_tile_atlas = m_map.tileset.atlases.emplace_back();
  gd_tile_atlas.name = _escape_name(tileset.get_meta().get_name());
  gd_tile_atlas.image_path = source_image_path;
  gd_tile_atlas.texture_id = texture_id;
  gd_tile_atlas.first_tile_id = tileset.get_first_tile_id();
  gd_tile_atlas.tile_count = static_cast<std::int32_t>(tileset.tile_count());
  gd_tile_atlas.column_count = static_cast<std::int32_t>(tileset.column_count());
  gd_tile_atlas.tile_size = tileset.get_tile_size();
  gd_tile_atlas.image_size = tileset.get_image_size();

  return {};
}

auto Gd3DocumentConverter::visit(const ITileView& tile) -> std::expected<void, std::error_code>
{
  if (tile.animation_frame_count() > 0) {
    const auto& tileset = tile.get_parent_tileset();
    const auto texture_res_id = m_map.tileset_texture_ids.at(tileset.get_first_tile_id());
    _convert_tile_animation(tile, m_map, texture_res_id);
  }

  return {};
}

auto Gd3DocumentConverter::get_map() const -> const Gd3Map&
{
  return m_map;
}

}  // namespace tactile::godot_tscn
