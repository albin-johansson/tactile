// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.layer:tile_layer;

export import :interfaces;
export import :layer_info;

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
