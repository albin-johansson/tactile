#include "tile_layer_item.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

using tactile::model::core;

namespace tactile::ui {

tile_layer_item::tile_layer_item(core* core, QGraphicsItem* parent)
    : QGraphicsItem{parent}, m_core{core}
{}

void tile_layer_item::paint(QPainter* painter,
                            const QStyleOptionGraphicsItem* option,
                            QWidget* widget)
{}

auto tile_layer_item::boundingRect() const -> QRectF
{
  return QRectF{};
}

// auto tile_layer_item::layer() noexcept -> model::tile_layer*
//{
//  return m_layer;
//}
//
// auto tile_layer_item::layer() const noexcept -> const model::tile_layer*
//{
//  return m_layer;
//}

}  // namespace tactile::ui
