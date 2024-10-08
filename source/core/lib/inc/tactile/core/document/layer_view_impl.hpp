// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/document/meta_view_impl.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

struct CTileFormat;
class MapDocument;
class ILayerView;

/**
 * A layer view implementation.
 */
class LayerViewImpl final : public ILayerView
{
 public:
  /**
   * Creates a layer view.
   *
   * \param document     The associated document.
   * \param parent_layer The parent layer, if any.
   * \param layer_id     The layer entity identifier.
   */
  LayerViewImpl(const MapDocument* document,
                const ILayerView* parent_layer,
                EntityID layer_id);

  [[nodiscard]]
  auto accept(IDocumentVisitor& visitor) const -> std::expected<void, ErrorCode> override;

  void write_tile_bytes(ByteStream& byte_stream) const override;

  [[nodiscard]]
  auto get_parent_layer() const -> const ILayerView* override;

  [[nodiscard]]
  auto get_id() const -> LayerID override;

  [[nodiscard]]
  auto get_type() const -> LayerType override;

  [[nodiscard]]
  auto get_opacity() const -> float override;

  [[nodiscard]]
  auto is_visible() const -> bool override;

  [[nodiscard]]
  auto get_global_index() const -> std::size_t override;

  [[nodiscard]]
  auto layer_count() const -> std::size_t override;

  [[nodiscard]]
  auto object_count() const -> std::size_t override;

  [[nodiscard]]
  auto get_tile(const Index2D& index) const -> std::optional<TileID> override;

  [[nodiscard]]
  auto get_tile_position_in_tileset(TileID tile_id) const -> std::optional<Index2D> override;

  [[nodiscard]]
  auto is_tile_animated(const Index2D& position) const -> bool override;

  [[nodiscard]]
  auto get_tile_encoding() const -> TileEncoding override;

  [[nodiscard]]
  auto get_tile_compression() const -> std::optional<CompressionFormat> override;

  [[nodiscard]]
  auto get_compression_level() const -> std::optional<int> override;

  [[nodiscard]]
  auto get_extent() const -> std::optional<Extent2D> override;

  [[nodiscard]]
  auto get_meta() const -> const IMetaView& override;

 private:
  const MapDocument* mDocument;
  const ILayerView* mParentLayer;
  EntityID mLayerId;
  MetaViewImpl mMeta;

  [[nodiscard]]
  auto _get_tile_format() const -> const CTileFormat&;
};

}  // namespace tactile
