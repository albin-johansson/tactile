#include "map_item.hpp"

#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOption>
#include <algorithm>  // min, max

#include "position.hpp"
#include "preferences.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

using namespace core;

namespace {

auto get_end_row(const QRectF& exposed, row_t nRows, int tileSize) -> row_t
{
  const auto endY = static_cast<int>(exposed.y() + exposed.height());
  const auto r = (endY / tileSize) + 1;
  return row_t{std::min(nRows.get(), r)};
}

auto get_end_col(const QRectF& exposed, col_t nCols, int tileSize) -> col_t
{
  const auto endX = static_cast<int>(exposed.x() + exposed.width());
  const auto c = (endX / tileSize) + 1;
  return col_t{std::min(nCols.get(), c)};
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

map_item::map_item(core::map_document* map, QGraphicsItem* parent)
    : QGraphicsItem{parent},
      m_map{map}
{
  if (!m_map) {
    throw tactile_error{"Cannot create map item based on null map document!"};
  }
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
  const auto endRow = get_end_row(exposed, m_map->row_count(), tileSize);
  const auto endCol = get_end_col(exposed, m_map->col_count(), tileSize);

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
  if (const auto* tilesets = m_map->tilesets()) {
    const auto& image = tilesets->image(tile);
    const auto src = tilesets->image_source(tile);
    const QRect dst{x, y, tileSize, tileSize};
    painter.drawPixmap(dst, image, src);
  }
}

void map_item::draw_preview_multiple_tiles(QPainter& painter,
                                           const position& mousePosition,
                                           const tileset::selection& selection,
                                           int tileSize)
{
  // TODO test rendering preview centered around mouse
  auto* tileset = m_map->current_tileset();
  Q_ASSERT(tileset);

  tileset->iterate_selection([&](position pos) {
    const auto tilePos = mousePosition + pos;
    if (m_map->in_bounds(tilePos)) {
      draw_tile(painter,
                tileset->tile_at(selection.topLeft + pos),
                tilePos.col_to_x(tileSize),
                tilePos.row_to_y(tileSize),
                tileSize);
    }
  });
}

void map_item::draw_preview(QPainter& painter, int tileSize)
{
  auto* tileset = m_map->current_tileset();
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

  const auto tileSize = m_map->current_tile_size();
  const auto& exposed = option->exposedRect;

  m_map->each_layer([&](const layer& layer) {
    if (layer.visible()) {
      draw_layer(*painter, layer, exposed, tileSize);
    }
  });

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
