// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/document/meta_view_impl.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

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

  void accept(IDocumentVisitor& visitor) const override;

  [[nodiscard]]
  auto get_parent_layer() const -> const ILayerView* override;

  [[nodiscard]]
  auto get_type() const -> LayerType override;

  [[nodiscard]]
  auto get_opacity() const -> float override;

  [[nodiscard]]
  auto is_visible() const -> bool override;

  [[nodiscard]]
  auto get_global_index() const -> usize override;

  [[nodiscard]]
  auto get_tile(const MatrixIndex& index) const -> Optional<TileID> override;

  [[nodiscard]]
  auto get_extent() const -> Optional<MatrixExtent> override;

  [[nodiscard]]
  auto get_meta() const -> const IMetaView& override;

 private:
  const MapDocument* mDocument;
  const ILayerView* mParentLayer;
  EntityID mLayerId;
  MetaViewImpl mMeta;
};

}  // namespace tactile
