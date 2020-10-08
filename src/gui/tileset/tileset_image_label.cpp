#include "tileset_image_label.hpp"

#include <qpainter.h>

namespace tactile::gui {

tileset_image_label::tileset_image_label(const core::tileset& tileset,
                                         QWidget* parent)
    : QLabel{parent},
      m_tileWidth{tileset.get_tile_width()},
      m_tileHeight{tileset.get_tile_height()}
{
  setPixmap(tileset.image());

  m_width = tileset.width();
  m_height = tileset.height();

  m_maxX = m_width - 1;
  m_maxY = m_height - 1;

  m_nRows = m_height / m_tileWidth.get();
  m_nCols = m_width / m_tileHeight.get();
}

tileset_image_label::~tileset_image_label() noexcept = default;

void tileset_image_label::paintEvent(QPaintEvent* event)
{
  QLabel::paintEvent(event);

  QPainter painter{this};
  painter.setPen(QPen{Qt::black, 1});

  const auto region = visibleRegion().boundingRect();

  const auto tileWidth = m_tileWidth.get();
  const auto tileHeight = m_tileHeight.get();

  const auto minRow = region.y() / tileHeight;
  const auto minCol = region.x() / tileWidth;
  const auto maxRow = ((region.y() + region.height()) / tileHeight) + 1;
  const auto maxCol = ((region.x() + region.width()) / tileWidth) + 1;

  for (auto row = minRow; row < maxRow; ++row) {
    for (auto col = minCol; col < maxCol; ++col) {
      const auto x = col * tileWidth;
      const auto y = row * tileHeight;
      painter.drawRect(x, y, m_width, m_height);
    }
  }

  {
    // renders the bottom and right-hand end lines
    painter.drawLine(0, m_maxY, m_width, m_maxY);
    painter.drawLine(m_maxX, 0, m_maxX, m_height);
  }
}

}  // namespace tactile::gui
