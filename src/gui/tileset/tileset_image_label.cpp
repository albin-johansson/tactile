#include "tileset_image_label.hpp"

#include <qpainter.h>

#include "algorithm.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

tileset_image_label::tileset_image_label(const QImage& image,
                                         tile_width tileWidth,
                                         tile_height tileHeight,
                                         QWidget* parent)
    : QLabel{parent},
      m_tileWidth{at_least(tileWidth, 1_tw)},
      m_tileHeight{at_least(tileHeight, 1_th)}
{
  if (image.isNull()) {
    throw tactile_error{"Can't create tileset image label from null image!"};
  }
  setPixmap(QPixmap::fromImage(image));

  m_width = image.width();
  m_height = image.height();

  m_maxX = m_width - 1;
  m_maxY = m_height - 1;

  m_nRows = m_height / m_tileWidth;
  m_nCols = m_width / m_tileHeight;
}

tileset_image_label::~tileset_image_label() noexcept = default;

void tileset_image_label::paintEvent(QPaintEvent* event)
{
  QLabel::paintEvent(event);

  QPainter painter{this};

  QPen pen;
  pen.setColor(Qt::black);
  pen.setWidth(1);
  painter.setPen(pen);

  const auto region = visibleRegion().boundingRect();

  const auto minRow = region.y() / m_tileHeight;
  const auto minCol = region.x() / m_tileWidth;
  const auto maxRow = ((region.y() + region.height()) / m_tileHeight) + 1;
  const auto maxCol = ((region.x() + region.width()) / m_tileWidth) + 1;

  for (auto row = minRow; row < maxRow; ++row) {
    for (auto col = minCol; col < maxCol; ++col) {
      const auto x = col * m_tileWidth;
      const auto y = row * m_tileHeight;
      painter.drawLine(x, 0, x, m_width);
      painter.drawLine(0, y, m_height, y);
    }
  }

  {
    // renders the bottom and right-hand end lines
    painter.drawLine(0, m_maxY, m_width, m_maxY);
    painter.drawLine(m_maxX, 0, m_maxX, m_height);
  }
}

}  // namespace tactile::gui
