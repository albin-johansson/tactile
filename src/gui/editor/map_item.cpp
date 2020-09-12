#include "map_item.hpp"

#include <qgraphicswidget.h>
#include <qpainter.h>
#include <qstyleoption.h>

#include <algorithm>  // min, max

#include "position.hpp"
#include "preferences.hpp"

namespace tactile::gui {

using namespace core;

namespace {

auto get_end_row(const QRectF& exposed, int nRows, int tileSize) -> row_t
{
  const auto endY = static_cast<int>(exposed.y() + exposed.height());
  const auto r = (endY / tileSize) + 1;
  return row_t{std::min(nRows, r)};
}

auto get_end_col(const QRectF& exposed, int nCols, int tileSize) -> col_t
{
  const auto endX = static_cast<int>(exposed.x() + exposed.width());
  const auto c = (endX / tileSize) + 1;
  return col_t{std::min(nCols, c)};
}

void draw_tile_background(QPainter& painter,
                          const position& position,
                          int tileSize)
{
  constexpr QColor emptyLightGray{0x55, 0x55, 0x55};
  constexpr QColor emptyDarkGray{0x33, 0x33, 0x33};

  const auto row = position.row();
  const auto col = position.col();

  const auto& color = ((row.get() % 2 == 0) == (col.get() % 2 == 0))
                          ? emptyDarkGray
                          : emptyLightGray;
  painter.fillRect(position.col_to_x(tileSize),
                   position.row_to_y(tileSize),
                   tileSize,
                   tileSize,
                   color);
}

}  // namespace

map_item::map_item(not_null<map*> map,
                   not_null<tileset_model*> tilesets,
                   QGraphicsItem* parent)
    : QGraphicsItem{parent},
      m_map{map},
      m_tilesets{tilesets}
{
  // This is to be able to use exposedRect
  setFlags(QGraphicsItem::ItemUsesExtendedStyleOption);
}

void map_item::disable_stamp_preview()
{
  m_mousePosition.reset();
  update();
}

void map_item::enable_stamp_preview(const position& position)
{
  m_mousePosition = position;
}

void map_item::draw_layer(QPainter& painter,
                          const layer& layer,
                          const QRectF& exposed,
                          int tileSize)
{
  if (exposed.isEmpty()) {
    return;
  }

  const auto beginRow = std::max(0, static_cast<int>(exposed.y() / tileSize));
  const auto beginCol = std::max(0, static_cast<int>(exposed.x() / tileSize));
  const auto endRow = get_end_row(exposed, m_map->rows(), tileSize);
  const auto endCol = get_end_col(exposed, m_map->cols(), tileSize);

  const auto renderGrid = prefs::graphics::render_grid().value_or(false);

  for (row_t row{beginRow}; row < endRow; ++row) {
    for (col_t col{beginCol}; col < endCol; ++col) {
      const position position{row, col};

      const auto x = position.col_to_x(tileSize);
      const auto y = position.row_to_y(tileSize);

      draw_tile_background(painter, position, tileSize);

      if (const auto tile = layer.tile_at(position); tile != empty) {
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

void map_item::draw_preview_multiple_tiles(QPainter& painter,
                                           const position& mousePosition,
                                           const tileset::selection& selection,
                                           int tileSize)
{
  // TODO test rendering preview centered around mouse

  auto* tileset = m_tilesets->current_tileset();
  Q_ASSERT(tileset);

  const auto& [topLeft, bottomRight] = selection;

  const auto nRows = 1_row + (bottomRight.row() - topLeft.row());
  const auto nCols = 1_col + (bottomRight.col() - topLeft.col());

  for (row_t row{0}; row < nRows; ++row) {
    for (col_t col{0}; col < nCols; ++col) {
      const auto tilePos = mousePosition.offset_by(row, col);
      if (m_map->in_bounds(tilePos)) {
        draw_tile(painter,
                  tileset->tile_at(topLeft.offset_by(row, col)),
                  tilePos.col_to_x(tileSize),
                  tilePos.row_to_y(tileSize),
                  tileSize);
      }
    }
  }
}

void map_item::draw_preview(QPainter& painter, int tileSize)
{
  auto* tileset = m_tilesets->current_tileset();
  Q_ASSERT(tileset);
  Q_ASSERT(tileset->get_selection());

  const auto& mousePos = *m_mousePosition;
  const auto& [topLeft, bottomRight] = *tileset->get_selection();

  painter.setOpacity(0.5);

  if (topLeft == bottomRight) {
    draw_tile(painter,
              tileset->tile_at(topLeft),
              mousePos.col_to_x(tileSize),
              mousePos.row_to_y(tileSize),
              tileSize);
  } else {
    draw_preview_multiple_tiles(
        painter, mousePos, *tileset->get_selection(), tileSize);
  }

  painter.setOpacity(1);
}

void map_item::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem* option,
                     QWidget*)
{
  Q_ASSERT(painter);
  Q_ASSERT(option);

  const auto tileSize = m_map->get_tile_size().get();

  const auto& exposed = option->exposedRect;
  for (const auto& layer : *m_map) {
    if (layer.visible()) {
      draw_layer(*painter, layer, exposed, tileSize);
    }
  }

  if (m_mousePosition) {
    draw_preview(*painter, tileSize);
  }
}

auto map_item::boundingRect() const -> QRectF
{
  const auto width = static_cast<qreal>(m_map->width());
  const auto height = static_cast<qreal>(m_map->height());
  return {0, 0, width, height};
}

}  // namespace tactile::gui
