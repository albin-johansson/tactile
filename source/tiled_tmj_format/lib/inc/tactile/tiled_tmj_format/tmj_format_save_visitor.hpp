// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

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
  auto visit(const IMapView& map) -> Result<void> override;

  [[nodiscard]]
  auto visit(const ITilesetView& tileset) -> Result<void> override;

  [[nodiscard]]
  auto visit(const ITileView& tile) -> Result<void> override;

  [[nodiscard]]
  auto visit(const ILayerView& layer) -> Result<void> override;

  [[nodiscard]]
  auto visit(const IObjectView& object) -> Result<void> override;

  [[nodiscard]]
  auto visit(const IComponentView& component) -> Result<void> override;

  [[nodiscard]]
  auto get_map_json() const -> const nlohmann::json&;

 private:
  IRuntime* mRuntime;
  SaveFormatWriteOptions mOptions;
  nlohmann::json mMapNode {};
  HashMap<TileID, nlohmann::json> mExternalTilesetNodes {};
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
