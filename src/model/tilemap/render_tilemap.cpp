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

[[nodiscard]] auto calc_bounds(const QRect& viewport,
                               int nRows,
                               int nCols,
                               int tileSize) noexcept -> render_bounds
{
  const auto minCol = (viewport.x() > 0) ? 0 : (-viewport.x() / tileSize);
  const auto minRow = (viewport.y() > 0) ? 0 : (-viewport.y() / tileSize);

  const auto calcMaxCol = (viewport.width() - viewport.x()) / tileSize + 1;
  const auto calcMaxRow = (viewport.height() - viewport.y()) / tileSize + 1;
  const auto maxCol = (calcMaxCol > nCols) ? nCols : calcMaxCol;
  const auto maxRow = (calcMaxRow > nRows) ? nRows : calcMaxRow;

  return {minRow, minCol, maxRow, maxCol};
}

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
  const auto renderGrid = settings_bool("visuals-grid").value_or(true);
  if (renderGrid) {
    painter.setPen(create_pen());
  }

  const auto tileSize = map.get_tile_size().get();
  const auto bounds =
      calc_bounds(painter.viewport(), map.rows(), map.cols(), tileSize);

  for (auto& layer : map) {
    if (layer.visible()) {
      render_layer(painter, layer, bounds, tileSize, renderGrid);
    }
  }
}

}  // namespace tactile
