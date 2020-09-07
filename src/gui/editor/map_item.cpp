#include "map_item.hpp"

#include <qgraphicswidget.h>
#include <qpainter.h>
#include <qstyleoption.h>

#include <algorithm>  // min, max

#include "position.hpp"
#include "preferences.hpp"

namespace tactile::gui {

using core::col;
using core::layer;
using core::map;
using core::row;

namespace {

auto get_end_row(const QRectF& exposed, int nRows, int tileSize) -> row
{
  const auto endY = static_cast<int>(exposed.y() + exposed.height());
  const auto r = (endY / tileSize) + 1;
  return row{std::min(nRows, r)};
}

auto get_end_col(const QRectF& exposed, int nCols, int tileSize) -> col
{
  const auto endX = static_cast<int>(exposed.x() + exposed.width());
  const auto c = (endX / tileSize) + 1;
  return col{std::min(nCols, c)};
}

void draw_tile_background(QPainter& painter, row r, col c, int tileSize)
{
  constexpr QColor emptyLightGray{0x55, 0x55, 0x55};
  constexpr QColor emptyDarkGray{0x33, 0x33, 0x33};
  const auto& color = ((r.get() % 2 == 0) == (c.get() % 2 == 0))
                          ? emptyDarkGray
                          : emptyLightGray;
  painter.fillRect(
      c.get() * tileSize, r.get() * tileSize, tileSize, tileSize, color);
}

}  // namespace

map_item::map_item(not_null<map*> map,
                   not_null<core::tileset_model*> tilesets,
                   QGraphicsItem* parent)
    : QGraphicsItem{parent}, m_map{map}, m_tilesets{tilesets}
{
  // This is to be able to use exposedRect
  setFlags(QGraphicsItem::ItemUsesExtendedStyleOption);
}

void map_item::draw_layer(QPainter& painter,
                          const layer& layer,
                          const QRectF& exposed)
{
  if (exposed.isEmpty()) {
    return;
  }

  const auto tileSize = m_map->get_tile_size().get();

  const auto beginRow = std::max(0, static_cast<int>(exposed.y() / tileSize));
  const auto beginCol = std::max(0, static_cast<int>(exposed.x() / tileSize));
  const auto endRow = get_end_row(exposed, m_map->rows(), tileSize);
  const auto endCol = get_end_col(exposed, m_map->cols(), tileSize);

  const auto renderGrid = prefs::graphics::render_grid().value_or(false);

  for (row row{beginRow}; row < endRow; ++row) {
    for (col col{beginCol}; col < endCol; ++col) {
      const auto x = col.get() * tileSize;
      const auto y = row.get() * tileSize;

      draw_tile_background(painter, row, col, tileSize);

      if (const auto tile = layer.tile_at({row, col}); tile != empty) {
        draw_tile(painter, *tile, x, y, tileSize);
      }

      if (renderGrid) {
        painter.drawRect(x, y, tileSize, tileSize);
      }
    }
  }
}

void map_item::draw_tile(QPainter& painter,
                         tile_id tile,
                         int x,
                         int y,
                         int tileSize)
{
  Q_ASSERT(m_tilesets->contains(tile));

  const auto& image = m_tilesets->image(tile);
  const auto src = m_tilesets->image_source(tile);
  const QRect dst{x, y, tileSize, tileSize};
  painter.drawPixmap(dst, image, src);
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
