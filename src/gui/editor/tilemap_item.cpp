#include "tilemap_item.hpp"

#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "setting.hpp"
#include "setting_identifiers.hpp"

namespace tactile::gui {

using model::tile_layer;
using model::tilemap;

tilemap_item::tilemap_item(not_null<tilemap*> map, QGraphicsItem* parent)
    : QGraphicsItem{parent}, m_map{map}
{}

void tilemap_item::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget*)
{
  Q_ASSERT(painter);
  Q_ASSERT(option);

  const auto& exposed = option->exposedRect;

  for (const auto& layer : *m_map) {
    if (layer.visible()) {
      draw_layer(*painter, layer, exposed);
    }
  }
}

void tilemap_item::draw_layer(QPainter& painter,
                              const tile_layer& layer,
                              const QRectF& exposed)
{
  const auto tileSize = m_map->get_tile_size().get();

  //  const QRect bounds{0, 0, layer.cols() * tileSize, layer.rows() *
  //  tileSize};
  const auto startX = exposed.left();
  const auto startY = exposed.top();
  //  const auto endX = bounds.right();
  //  const auto endY = bounds.bottom();

  const int minCol = 0;
  const int minRow = 0;
  const int maxCol = layer.cols();
  const int maxRow = layer.rows();

  constexpr QColor emptyGray{0x55, 0x55, 0x55};

  const auto grid = prefs::setting<bool>{prefs::id::graphics::grid()};
  const auto renderGrid = grid.value_or(true);

  for (auto row = minRow; row < maxRow; ++row) {
    for (auto col = minCol; col < maxCol; ++col) {
      const auto x = startX + (col * tileSize);
      const auto y = startY + (row * tileSize);
      painter.fillRect(x, y, tileSize, tileSize, emptyGray);
      if (renderGrid) {
        painter.drawRect(x, y, tileSize, tileSize);
      }
    }
  }
}

auto tilemap_item::boundingRect() const -> QRectF
{
  const qreal width = m_map->width();
  const qreal height = m_map->height();
  return {0, 0, width, height};
}

}  // namespace tactile::gui
