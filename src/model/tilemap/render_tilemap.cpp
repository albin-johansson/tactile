#include "render_tilemap.hpp"

#include <QColor>
#include <QPainter>
#include <QSettings>
#include <algorithm>

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
//  //  const auto calcMaxCol = (viewport.width() - viewport.x()) / tileSize +
//  1;
//  //  const auto calcMaxRow = (viewport.height() - viewport.y()) / tileSize +
//  1;
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

void render_tile(QPainter& painter)
{}

void render_layer(QPainter& painter,
                  const tile_layer& layer,
                  const QRectF& exposed,
                  const int tileSize)
{
//  const QRect bounds{0, 0, layer.cols() * tileSize, layer.rows() * tileSize};
  const auto startX = exposed.left();
  const auto startY = exposed.top();
//  const auto endX = bounds.right();
//  const auto endY = bounds.bottom();

  const int minCol = 0;
  const int minRow = 0;
  const int maxCol = layer.cols();
  const int maxRow = layer.rows();

  constexpr QColor emptyGray{0x55, 0x55, 0x55};

  for (auto row = minRow; row < maxRow; ++row) {
    for (auto col = minCol; col < maxCol; ++col) {
      const auto x = startX + (col * tileSize);
      const auto y = startY + (row * tileSize);
      painter.fillRect(x, y, tileSize, tileSize, emptyGray);
      //      if (renderGrid) {
      //        painter.drawRect(x, y, tileSize, tileSize);
      //      }
    }
  }
}

}  // namespace

void render_tilemap(QPainter& painter,
                    const tilemap& map,
                    const QRectF& exposed)
{
  const auto tileSize = map.get_tile_size().get();
  for (const auto& layer : map) {
    if (layer.visible()) {
      render_layer(painter, layer, exposed, tileSize);
    }
  }
}

}  // namespace tactile::model
