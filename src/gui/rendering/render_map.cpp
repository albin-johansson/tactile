#include "render_map.hpp"

#include <algorithm>  // min, max

#include "layer_utils.hpp"
#include "map.hpp"
#include "map_document.hpp"
#include "object_layer.hpp"
#include "preferences.hpp"
#include "tile_layer.hpp"
#include "tileset_manager.hpp"

namespace tactile {
namespace {

inline constexpr double previewOpacity = 0.5;

struct render_bounds final
{
  row_t rowBegin;
  row_t rowEnd;
  col_t colBegin;
  col_t colEnd;
};

struct render_info final
{
  const core::map* map;
  const core::tileset_manager* tilesets;
  render_bounds bounds;
  maybe<core::position> mousePosition;
  int tileSize;
  bool drawGrid;
};

[[nodiscard]] auto get_end_row(const QRectF& exposed,
                               const row_t nRows,
                               const int tileSize) -> row_t
{
  const auto endY = static_cast<int>(exposed.y() + exposed.height());
  const auto r = (endY / tileSize) + 1;
  return row_t{std::min(nRows.get(), r)};
}

[[nodiscard]] auto get_end_col(const QRectF& exposed,
                               const col_t nCols,
                               const int tileSize) -> col_t
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

[[nodiscard]] auto make_render_info(const core::map_document& document,
                                    const QRectF& exposed,
                                    maybe<core::position> mousePosition)
    -> render_info
{
  const auto tileSize = document.current_tile_size();
  const auto nRows = document.row_count();
  const auto nCols = document.col_count();

  return {.map = document.get_map(),
          .tilesets = document.tilesets(),
          .bounds = make_bounds(exposed, nRows, nCols, tileSize),
          .mousePosition = mousePosition,
          .tileSize = tileSize,
          .drawGrid = prefs::gfx::render_grid().value_or(false)};
}

void render_cell(QPainter& painter,
                 const core::position& position,
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

void render_tile(QPainter& painter,
                 const tile_id id,
                 const QPoint& position,
                 const render_info& info)
{
  const auto& image = info.tilesets->image(id);
  const auto src = info.tilesets->image_source(id);
  const QRect dst{position.x(), position.y(), info.tileSize, info.tileSize};
  painter.drawPixmap(dst, image, src);
}

void render_tile(QPainter& painter,
                 const tile_id id,
                 const core::position& position,
                 const render_info& info)
{
  const QPoint dst{position.col_to_x(info.tileSize),
                   position.row_to_y(info.tileSize)};
  render_tile(painter, id, dst, info);
}

void render_tile_layer(QPainter& painter,
                       const core::tile_layer& layer,
                       const render_info& info)
{
  painter.setOpacity(layer.Opacity());

  for (auto row = info.bounds.rowBegin; row < info.bounds.rowEnd; ++row)
  {
    for (auto col = info.bounds.colBegin; col < info.bounds.colEnd; ++col)
    {
      const core::position position{row, col};
      const auto x = position.col_to_x(info.tileSize);
      const auto y = position.row_to_y(info.tileSize);

      if (const auto id = layer.tile_at(position); id && id != empty)
      {
        const QPoint dst{x, y};
        render_tile(painter, *id, dst, info);
      }

      if (info.drawGrid)
      {  // FIXME don't render this as separate rects, draw
         // lines once
        painter.drawRect(x, y, info.tileSize, info.tileSize);
      }
    }
  }

  painter.setOpacity(1);
}

void render_object_layer(QPainter& painter,
                         const core::object_layer& layer,
                         const render_info& info)
{
  painter.setOpacity(layer.Opacity());

  layer.each_object([&](const object_id, const core::object& object) {
    painter.setPen(Qt::red);

    if (object.is_rectangle())
    {
      painter.drawRect(
          QRectF{object.x(), object.y(), object.width(), object.height()});
    }
    else if (object.is_point())
    {
      painter.drawEllipse(QPointF{object.x(), object.y()}, 2, 2);
    }
  });

  painter.setOpacity(1);
}

void render_background(QPainter& painter, const render_info& info)
{
  for (auto row = info.bounds.rowBegin; row < info.bounds.rowEnd; ++row)
  {
    for (auto col = info.bounds.colBegin; col < info.bounds.colEnd; ++col)
    {
      const core::position position{row, col};
      render_cell(painter, position, info.tileSize);
    }
  }
}

void render_layer(QPainter& painter,
                  const shared<core::ILayer>& layer,
                  const render_info& info)
{
  if (!layer->IsVisible())
  {
    return;
  }

  if (auto* tileLayer = core::as_tile_layer(layer))
  {
    render_tile_layer(painter, *tileLayer, info);
  }
  else if (auto* objectLayer = core::as_object_layer(layer))
  {
    render_object_layer(painter, *objectLayer, info);
  }
}

void render_multi_preview(QPainter& painter, const render_info& info)
{
  auto* tileset = info.tilesets->current_tileset();
  Q_ASSERT(tileset);

  // TODO take scale into consideration

  const auto selection = tileset->get_selection().value();
  const auto mousePos = info.mousePosition.value();

  const auto diff = selection.bottomRight - selection.topLeft;
  const core::position offset{diff.row() / 2_row, diff.col() / 2_col};

  tileset->iterate_selection([&](const core::position pos) {
    const auto tilePos = mousePos + pos - offset;
    if (info.map->in_bounds(tilePos))
    {
      render_tile(painter,
                  tileset->tile_at(selection.topLeft + pos),
                  tilePos,
                  info);
    }
  });
}

void render_preview(QPainter& painter, const render_info& info)
{
  auto* tileset = info.tilesets->current_tileset();

  if (!tileset)
  {
    return;
  }

  painter.setOpacity(previewOpacity);

  const auto [topLeft, bottomRight] = tileset->get_selection().value();
  if (topLeft == bottomRight)
  {
    render_tile(painter,
                tileset->tile_at(topLeft),
                info.mousePosition.value(),
                info);
  }
  else
  {
    render_multi_preview(painter, info);
  }

  painter.setOpacity(1);
}

}  // namespace

void RenderMap(QPainter& painter,
               const core::map_document& document,
               const maybe<core::position> mousePosition,
               const QRectF& exposed)
{
  if (exposed.isEmpty())
  {
    return;
  }

  const auto info = make_render_info(document, exposed, mousePosition);
  render_background(painter, info);

  document.each_layer([&](const layer_id id, const shared<core::ILayer>& layer) {
    render_layer(painter, layer, info);

    if (info.mousePosition && id == document.current_layer_id())
    {
      render_preview(painter, info);
    }
  });
}

}  // namespace tactile
