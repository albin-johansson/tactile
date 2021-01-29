#include "map_item.hpp"

#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOption>
#include <algorithm>  // min, max

#include "map.hpp"
#include "map_document.hpp"
#include "position.hpp"
#include "preferences.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::gui {

using namespace core;

namespace {

auto get_end_row(const QRectF& exposed, const row_t nRows, const int tileSize)
    -> row_t
{
  const auto endY = static_cast<int>(exposed.y() + exposed.height());
  const auto r = (endY / tileSize) + 1;
  return row_t{std::min(nRows.get(), r)};
}

auto get_end_col(const QRectF& exposed, const col_t nCols, const int tileSize)
    -> col_t
{
  const auto endX = static_cast<int>(exposed.x() + exposed.width());
  const auto c = (endX / tileSize) + 1;
  return col_t{std::min(nCols.get(), c)};
}

[[nodiscard]] auto make_bounds(const QRectF& exposed,
                               const row_t rows,
                               const col_t cols,
                               const int tileSize) -> render_bounds
{
  render_bounds bounds;

  bounds.rowBegin =
      row_t{std::max(0, static_cast<int>(exposed.y() / tileSize))};
  bounds.colBegin =
      col_t{std::max(0, static_cast<int>(exposed.x() / tileSize))};
  bounds.rowEnd = get_end_row(exposed, rows, tileSize);
  bounds.colEnd = get_end_col(exposed, cols, tileSize);

  return bounds;
}

void draw_tile_background(QPainter& painter,
                          const position& position,
                          const int tileSize)
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
    : QGraphicsItem{parent}
    , m_map{map}
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

auto map_item::make_settings(const QRectF& exposed, const int tileSize)
    -> render_settings
{
  render_settings settings;

  settings.bounds =
      make_bounds(exposed, m_map->row_count(), m_map->col_count(), tileSize);
  settings.drawGrid = prefs::graphics::render_grid().value_or(false);
  settings.tileSize = m_map->current_tile_size();

  return settings;
}

void map_item::draw_background(QPainter& painter,
                               const render_settings& settings)
{
  each_tile(painter, settings.bounds, [&](position position) {
    draw_tile_background(painter, position, settings.tileSize);
  });
}

void map_item::draw_tile_layer(QPainter& painter,
                               const tile_layer& layer,
                               const render_settings& bounds)
{
  painter.setOpacity(layer.opacity());
  each_tile(painter, bounds.bounds, [&, this](const position position) {
    const auto x = position.col_to_x(bounds.tileSize);
    const auto y = position.row_to_y(bounds.tileSize);

    if (const auto tile = layer.tile_at(position); tile && tile != empty) {
      draw_tile(painter, *tile, x, y, bounds.tileSize);
    }

    if (bounds.drawGrid) {
      painter.drawRect(x, y, bounds.tileSize, bounds.tileSize);
    }
  });
  painter.setOpacity(1);
}

void map_item::draw_tile(QPainter& painter,
                         const tile_id tile,
                         const int x,
                         const int y,
                         const int tileSize)
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
                                           const int tileSize)
{
  auto* tileset = m_map->current_tileset();
  Q_ASSERT(tileset);

  const auto diff = selection.bottomRight - selection.topLeft;
  const position offset{diff.row() / 2_row, diff.col() / 2_col};

  tileset->iterate_selection([&](position pos) {
    const auto tilePos = mousePosition + pos - offset;
    if (m_map->in_bounds(tilePos)) {
      draw_tile(painter,
                tileset->tile_at(selection.topLeft + pos),
                tilePos.col_to_x(tileSize),
                tilePos.row_to_y(tileSize),
                tileSize);
    }
  });
}

void map_item::draw_preview(QPainter& painter, const int tileSize)
{
  auto* tileset = m_map->current_tileset();
  Q_ASSERT(tileset);

  const auto& mousePos = m_mousePosition.value();
  const auto& [topLeft, bottomRight] = tileset->get_selection().value();

  painter.setOpacity(m_previewOpacity);

  if (topLeft == bottomRight) {
    draw_tile(painter,
              tileset->tile_at(topLeft),
              mousePos.col_to_x(tileSize),
              mousePos.row_to_y(tileSize),
              tileSize);
  } else {
    draw_preview_multiple_tiles(painter,
                                mousePos,
                                *tileset->get_selection(),
                                tileSize);
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
  if (exposed.isEmpty()) {
    return;
  }

  const auto settings = make_settings(exposed, tileSize);
  draw_background(*painter, settings);

  const auto activeLayer = m_map->current_layer_id();

  m_map->each_layer([&](const layer_id id, const shared_layer& layer) {
    if (layer->visible()) {
      if (const auto* tileLayer =
              dynamic_cast<const core::tile_layer*>(layer.get())) {
        draw_tile_layer(*painter, *tileLayer, settings);
      } else {
        // TODO render object layer
      }
    }

    if (m_mousePosition && activeLayer == id) {
      draw_preview(*painter, tileSize);
    }
  });
}

auto map_item::boundingRect() const -> QRectF
{
  const auto width = static_cast<qreal>(m_map->width());
  const auto height = static_cast<qreal>(m_map->height());
  return {0, 0, width, height};
}

}  // namespace tactile::gui
