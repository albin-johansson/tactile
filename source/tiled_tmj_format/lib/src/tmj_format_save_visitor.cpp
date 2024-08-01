// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_save_visitor.hpp"

#include <stdexcept>     // runtime_error
#include <system_error>  // make_error_code, errc
#include <utility>       // move

#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/document/map_view.hpp"
#include "tactile/base/document/meta_view.hpp"
#include "tactile/base/document/object_view.hpp"
#include "tactile/base/document/tile_view.hpp"
#include "tactile/base/document/tileset_view.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj_format/tmj_format_layer_emitter.hpp"
#include "tactile/tiled_tmj_format/tmj_format_map_emitter.hpp"
#include "tactile/tiled_tmj_format/tmj_format_object_emitter.hpp"
#include "tactile/tiled_tmj_format/tmj_format_tile_emitter.hpp"
#include "tactile/tiled_tmj_format/tmj_format_tileset_emitter.hpp"

namespace tactile {

TmjFormatSaveVisitor::TmjFormatSaveVisitor(IRuntime* runtime, SaveFormatWriteOptions options)
  : mRuntime {runtime},
    mOptions {std::move(options)}
{}

auto TmjFormatSaveVisitor::visit(const IMapView& map) -> Result<void>
{
  mMapNode = emit_tiled_tmj_map(map);

  if (mOptions.use_external_tilesets) {
    mExternalTilesetNodes.reserve(map.tileset_count());
  }

  return kOK;
}

auto TmjFormatSaveVisitor::visit(const ITilesetView& tileset) -> Result<void>
{
  auto tileset_json = emit_tiled_tmj_tileset(tileset, mOptions, mExternalTilesetNodes);

  mMapNode.at("tilesets").push_back(std::move(tileset_json));

  return kOK;
}

auto TmjFormatSaveVisitor::visit(const ITileView& tile) -> Result<void>
{
  auto tile_json = emit_tiled_tmj_tile(tile);

  const auto& tileset = tile.get_parent_tileset();
  auto& tileset_json = _get_tileset_json(tileset);
  tileset_json.at("tiles").push_back(std::move(tile_json));

  return kOK;
}

auto TmjFormatSaveVisitor::visit(const ILayerView& layer) -> Result<void>
{
  auto layer_json = emit_tiled_tmj_layer(*mRuntime, layer, mTileByteCache);

  if (!layer_json.has_value()) {
    return propagate_unexpected(layer_json);
  }

  if (const auto* parent_layer = layer.get_parent_layer()) {
    auto& parent_layer_json = _get_layer_json(*parent_layer);
    parent_layer_json.at("layers").push_back(std::move(*layer_json));
  }
  else {
    mMapNode.at("layers").push_back(std::move(*layer_json));
  }

  return kOK;
}

auto TmjFormatSaveVisitor::visit(const IObjectView& object) -> Result<void>
{
  auto object_json = emit_tiled_tmj_object(object);

  if (const auto* parent_layer = object.get_parent_layer()) {
    auto& parent_layer_json = _get_layer_json(*parent_layer);
    parent_layer_json.at("objects").push_back(std::move(object_json));
  }
  else if (const auto* parent_tile = object.get_parent_tile()) {
    auto& tile_json = _get_tile_json(*parent_tile);
    tile_json.at("objectgroup").at("objects").push_back(std::move(object_json));
  }
  else {
    log(LogLevel::kError, "Object {} has no parent layer or tile", object.get_id());
    return unexpected(std::make_error_code(std::errc::invalid_argument));
  }

  return kOK;
}

auto TmjFormatSaveVisitor::visit(const IComponentView&) -> Result<void>
{
  return kOK;
}

auto TmjFormatSaveVisitor::get_map_json() const -> const nlohmann::json&
{
  return mMapNode;
}

auto TmjFormatSaveVisitor::get_external_tilesets() const
    -> const HashMap<TileID, TmjFormatExternalTilesetData>&
{
  return mExternalTilesetNodes;
}

auto TmjFormatSaveVisitor::_get_tile_json(const ITileView& tile) -> nlohmann::json&
{
  const auto& tileset = tile.get_parent_tileset();
  auto& tileset_json = _get_tileset_json(tileset);

  if (auto* tile_json = _find_tile_json(tileset_json, tile.get_index())) {
    return *tile_json;
  }

  throw std::runtime_error {"no such tile node"};
}

auto TmjFormatSaveVisitor::_get_tileset_json(const ITilesetView& tileset) -> nlohmann::json&
{
  if (auto* tileset_json = _find_tileset_json(tileset.get_first_tile_id())) {
    return *tileset_json;
  }

  throw std::runtime_error {"no such tileset node"};
}

auto TmjFormatSaveVisitor::_get_layer_json(const ILayerView& layer) -> nlohmann::json&
{
  if (auto* layer_json = _find_layer_json(mMapNode, layer.get_id())) {
    return *layer_json;
  }

  throw std::runtime_error {"no such layer node"};
}

auto TmjFormatSaveVisitor::_find_layer_json(nlohmann::json& root_node,
                                            const LayerID id) -> nlohmann::json*
{
  if (!root_node.contains("layers")) {
    return nullptr;
  }

  for (const auto& [_, layer_node] : root_node.at("layers").items()) {
    if (layer_node.at("id") == id) {
      return &layer_node;
    }

    if (auto* found_node = _find_layer_json(layer_node, id)) {
      return found_node;
    }
  }

  return nullptr;
}

auto TmjFormatSaveVisitor::_find_tileset_json(const TileID first_tile_id) -> nlohmann::json*
{
  if (mOptions.use_external_tilesets) {
    const auto external_tileset_iter = mExternalTilesetNodes.find(first_tile_id);
    if (external_tileset_iter != mExternalTilesetNodes.end()) {
      return &external_tileset_iter->second.json;
    }
  }
  else {
    for (const auto& [_, tileset_node] : mMapNode.at("tilesets").items()) {
      if (tileset_node.at("firstgid") == first_tile_id) {
        return &tileset_node;
      }
    }
  }

  return nullptr;
}

auto TmjFormatSaveVisitor::_find_tile_json(nlohmann::json& tileset_json,
                                           const TileIndex tile_index) -> nlohmann::json*
{
  for (const auto& [_, tile_json] : tileset_json.at("tiles").items()) {
    if (tile_json.at("id") == tile_index) {
      return &tile_json;
    }
  }

  return nullptr;
}

}  // namespace tactile
