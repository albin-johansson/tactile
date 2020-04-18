#include "tilemap_renderer.h"

#include <QColor>
#include <QPainter>
#include <QSettings>

#include "settings_utils.h"
#include "tile_size.h"
#include "tilemap.h"

namespace tactile {
namespace {

struct RenderBounds final {
  int minRow = 0;
  int minCol = 0;
  int maxRow = 0;
  int maxCol = 0;
};

[[nodiscard]] RenderBounds calc_render_data(const QRect& viewport,
                                            int nRows,
                                            int nCols,
                                            int tileSize) noexcept
{
  const auto minCol = (viewport.x() > 0) ? 0 : (-viewport.x() / tileSize);
  const auto minRow = (viewport.y() > 0) ? 0 : (-viewport.y() / tileSize);

  const auto calcMaxCol = (viewport.width() - viewport.x()) / tileSize + 1;
  const auto calcMaxRow = (viewport.height() - viewport.y()) / tileSize + 1;
  const auto maxCol = (calcMaxCol > nCols) ? nCols : calcMaxCol;
  const auto maxRow = (calcMaxRow > nRows) ? nRows : calcMaxRow;

  return {minRow, minCol, maxRow, maxCol};
}

[[nodiscard]] QPen create_pen() noexcept
{
  QPen pen;
  pen.setColor(Qt::black);
  pen.setWidth(1);
  pen.setDashOffset(0);
  pen.setDashPattern({3, 4});
  return pen;
}

}  // namespace

void TilemapRenderer::render(QPainter& painter,
                             const Tilemap& map) const noexcept
{
  const auto tileSize = map.tile_size().size();
  const auto bounds =
      calc_render_data(painter.viewport(), map.rows(), map.cols(), tileSize);

  const auto renderGrid = settings_bool("visuals-grid").value_or(true);

  if (renderGrid) {
    painter.setPen(create_pen());
  }

  const QColor emptyGray{0x55, 0x55, 0x55};

  for (auto& layer : map.m_layers) {
    if (!layer.visible()) {
      continue;
    }

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
}

}  // namespace tactile
