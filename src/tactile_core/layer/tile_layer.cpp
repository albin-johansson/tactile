// Copyright (C) 2025 Albin Johansson

module tactile.core.layer;

namespace tactile {

TileLayer::TileLayer(const LayerID id)
  : m_info {id}
{}

void TileLayer::visit(ILayerVisitor& visitor)
{
  visitor.on_tile_layer(*this);
}

void TileLayer::visit(IConstLayerVisitor& visitor) const
{
  visitor.on_tile_layer(*this);
}

auto TileLayer::info() -> LayerInfo&
{
  return m_info;
}

auto TileLayer::info() const -> const LayerInfo&
{
  return m_info;
}

}  // namespace tactile
