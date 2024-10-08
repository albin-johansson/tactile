// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <unordered_map>  // unordered_map

#include <nlohmann/json.hpp>

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"
#include "tactile/tiled_tmj/tmj_format_tileset_emitter.hpp"

namespace tactile {

class IRuntime;
class IMetaView;

/**
 * A document visitor that produces Tiled TMJ format JSON objects.
 */
class TACTILE_TMJ_FORMAT_API TmjFormatSaveVisitor final : public IDocumentVisitor
{
 public:
  /**
   * Creates a visitor.
   *
   * \param runtime The associated runtime, cannot be null.
   * \param options The write options to use.
   */
  TmjFormatSaveVisitor(IRuntime* runtime, SaveFormatWriteOptions options);

  [[nodiscard]]
  auto visit(const IMapView& map) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const ITilesetView& tileset) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const ITileView& tile) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const ILayerView& layer) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const IObjectView& object) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto visit(const IComponentView& component) -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto get_map_json() const -> const nlohmann::json&;

  [[nodiscard]]
  auto get_external_tilesets() const
      -> const std::unordered_map<TileID, TmjFormatExternalTilesetData>&;

 private:
  IRuntime* mRuntime;
  SaveFormatWriteOptions mOptions;
  nlohmann::json mMapNode {};
  std::unordered_map<TileID, TmjFormatExternalTilesetData> mExternalTilesetNodes {};
  ByteStream mTileByteCache {};

  [[nodiscard]]
  auto _get_tile_json(const ITileView& tile) -> nlohmann::json&;

  [[nodiscard]]
  auto _get_tileset_json(const ITilesetView& tileset) -> nlohmann::json&;

  [[nodiscard]]
  auto _get_layer_json(const ILayerView& layer) -> nlohmann::json&;

  [[nodiscard]]
  static auto _find_layer_json(nlohmann::json& root_node, LayerID id) -> nlohmann::json*;

  [[nodiscard]]
  auto _find_tileset_json(TileID first_tile_id) -> nlohmann::json*;

  [[nodiscard]]
  auto _find_tile_json(nlohmann::json& tileset_json, TileIndex tile_index) -> nlohmann::json*;
};

}  // namespace tactile
