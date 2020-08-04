#include "render_tilemap.hpp"

#include <QColor>
#include <QPainter>
#include <QSettings>

#include "settings_utils.hpp"
#include "tile_size.hpp"
#include "tilemap.hpp"

namespace tactile::model {
namespace {

struct render_bounds final {
  int minRow{};
  int minCol{};
  int maxRow{};
  int maxCol{};
};

//[[nodiscard]] auto calc_bounds(const QRect& viewport,
//                               int nRows,
//                               int nCols,
//                               int tileSize) noexcept -> render_bounds
//{
//  const auto minCol = (viewport.x() > 0) ? 0 : (-viewport.x() / tileSize);
//  const auto minRow = (viewport.y() > 0) ? 0 : (-viewport.y() / tileSize);
//
//  //  const auto calcMaxCol = (viewport.width() - viewport.x()) / tileSize + 1;
//  //  const auto calcMaxRow = (viewport.height() - viewport.y()) / tileSize + 1;
//  //  const auto maxCol = (calcMaxCol > nCols) ? nCols : calcMaxCol;
//  //  const auto maxRow = (calcMaxRow > nRows) ? nRows : calcMaxRow;
//
//  const auto maxCol = (viewport.width() / tileSize) + 1;
//  const auto maxRow = (viewport.height() / tileSize) + 1;
//
//  return {minRow, minCol, maxRow, maxCol};
//}

[[nodiscard]] auto create_pen() noexcept -> QPen
{
  QPen pen{Qt::black};
  pen.setWidth(1);
  pen.setDashOffset(0);
  pen.setDashPattern({3, 4});
  return pen;
}

void render_layer(QPainter& painter,
                  const tile_layer& layer,
                  const render_bounds& bounds,
                  const int tileSize,
                  const bool renderGrid) noexcept
{
  constexpr QColor emptyGray{0x55, 0x55, 0x55};

  for (auto row = bounds.minRow; row < bounds.maxRow; ++row) {
    for (auto col = bounds.minCol; col < bounds.maxCol; ++col) {
      const auto x = col * tileSize;
      const auto y = row * tileSize;
      painter.fillRect(x, y, tileSize, tileSize, emptyGray);
      if (renderGrid) {
        painter.drawRect(x, y, tileSize, tileSize);
      }
    }
  }
}

}  // namespace

void render_tilemap(QPainter& painter, const tilemap& map) noexcept
{
  const auto tileSize = map.get_tile_size().get();
  const auto& viewport = painter.viewport();

//  auto minCol = viewport.x() / tileSize;
//  auto minRow = viewport.y() / tileSize;
//  auto maxCol = ((viewport.x() + viewport.width()) / tileSize) + 1;
//  auto maxRow = ((viewport.y() + viewport.height()) / tileSize) + 1;
//
//  if (minCol < 0) {
//    minCol = 0;
//  }
//
//  if (minRow < 0) {
//    minRow = 0;
//  }
//
//  if (maxRow > map.rows()) {
//    maxRow = map.rows();
//  }
//
//  if (maxCol > map.cols()) {
//    maxCol = map.cols();
//  }

  const render_bounds bounds{0, 0, 5, 5};

  const auto renderGrid = settings_bool("visuals-grid").value_or(true);
  if (renderGrid) {
    painter.setPen(create_pen());
  }

  for (auto& layer : map) {
    if (layer.visible()) {
      render_layer(painter, layer, bounds, tileSize, renderGrid);
    }
  }

  //  qDebug("---");
  //  qDebug("Map rows: %i, cols: %i", map.rows(), map.cols());
  //
  //  const auto tileSize = map.get_tile_size().get();
  //  const auto bounds =
  //      calc_bounds(painter.viewport(), map.rows(), map.cols(), tileSize);
  //
  //  qDebug("minCol: %i | minRow: %i | maxCol: %i | maxRow: %i",
  //         bounds.minCol,
  //         bounds.minRow,
  //         bounds.maxCol,
  //         bounds.maxRow);
  //  //  qInfo("Tile size: %i", tileSize);
  //
  //  //  qInfo("min row: %i, min col: %i, max row: %i, max col: %i",
  //  //        bounds.minRow,
  //  //        bounds.minCol,
  //  //        bounds.maxRow,
  //  //        bounds.maxCol);
  //
  //  for (auto& layer : map) {
  //    if (layer.visible()) {
  //      render_layer(painter, layer, bounds, tileSize, renderGrid);
  //    }
  //  }
}

}  // namespace tactile::model
