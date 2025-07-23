// Copyright (C) 2025 Albin Johansson

export module tactile.core.layer:tile_layer;

import :interfaces;
import :layer_info;

export namespace tactile {

/// A layer variant consisting of a grid of tile identifiers.
class TileLayer final : public ILayer
{
 public:
  explicit TileLayer(LayerID id);

  void visit(ILayerVisitor& visitor) override;

  void visit(IConstLayerVisitor& visitor) const override;

  [[nodiscard]]
  auto info() -> LayerInfo& override;

  [[nodiscard]]
  auto info() const -> const LayerInfo& override;

 private:
  LayerInfo m_info;
};

}  // namespace tactile
