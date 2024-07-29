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
  auto accept(IDocumentVisitor& visitor) const -> Result<void> override;

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
  auto get_global_index() const -> usize override;

  [[nodiscard]]
  auto layer_count() const -> usize override;

  [[nodiscard]]
  auto object_count() const -> usize override;

  [[nodiscard]]
  auto get_tile(const MatrixIndex& index) const -> Optional<TileID> override;

  [[nodiscard]]
  auto get_tile_encoding() const -> TileEncoding override;

  [[nodiscard]]
  auto get_tile_compression() const -> Optional<CompressionFormat> override;

  [[nodiscard]]
  auto get_compression_level() const -> Optional<int> override;

  [[nodiscard]]
  auto get_extent() const -> Optional<MatrixExtent> override;

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
