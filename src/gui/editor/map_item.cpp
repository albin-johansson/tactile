#include "map_item.hpp"

#include <qgraphicswidget.h>
#include <qpainter.h>
#include <qstyleoption.h>

#include <algorithm>  // min, max

#include "position.hpp"
#include "preferences.hpp"

namespace tactile::gui {

using model::layer;
using model::tilemap;

map_item::map_item(not_null<tilemap*> map, QGraphicsItem* parent)
    : QGraphicsItem{parent}, m_map{map}
{
  // This is to be able to use exposedRect
  setFlags(QGraphicsItem::ItemUsesExtendedStyleOption);
}

void map_item::draw_layer(QPainter& painter,
                          const layer& layer,
                          const QRectF& exposed)
{
  using model::col;
  using model::row;

  if (exposed.isEmpty()) {
    return;
  }

  const auto tileSize = m_map->get_tile_size().get();

  const auto get_end_row = [&exposed, tileSize](int nRows) -> row {
    const auto endY = static_cast<int>(exposed.y() + exposed.height());
    const auto row = (endY / tileSize) + 1;
    return model::row{std::min(nRows, row)};
  };

  const auto get_end_col = [&exposed, tileSize](int nCols) -> col {
    const auto endX = static_cast<int>(exposed.x() + exposed.width());
    const auto col = (endX / tileSize) + 1;
    return model::col{std::min(nCols, col)};
  };

  const auto beginRow = std::max(0, static_cast<int>(exposed.y() / tileSize));
  const auto beginCol = std::max(0, static_cast<int>(exposed.x() / tileSize));
  const auto endRow = get_end_row(m_map->rows());
  const auto endCol = get_end_col(m_map->cols());

  constexpr QColor emptyLightGray{0x55, 0x55, 0x55};
  constexpr QColor emptyDarkGray{0x33, 0x33, 0x33};
  const auto renderGrid = prefs::graphics::render_grid().value_or(false);

  for (row row{beginRow}; row < endRow; ++row) {
    for (col col{beginCol}; col < endCol; ++col) {
      const auto x = col.get() * tileSize;
      const auto y = row.get() * tileSize;

      const auto& color = ((row.get() % 2 == 0) == (col.get() % 2 == 0))
                              ? emptyDarkGray
                              : emptyLightGray;

      painter.fillRect(x, y, tileSize, tileSize, color);
      if (renderGrid) {
        painter.drawRect(x, y, tileSize, tileSize);
      }
    }
  }
}

void map_item::paint(QPainter* painter,
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

auto map_item::boundingRect() const -> QRectF
{
  const auto width = static_cast<qreal>(m_map->width());
  const auto height = static_cast<qreal>(m_map->height());
  return {0, 0, width, height};
}

}  // namespace tactile::gui
