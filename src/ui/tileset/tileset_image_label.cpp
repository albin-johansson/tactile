#include "tileset_image_label.h"

#include <QPainter>

#include "tactile_types.h"

namespace tactile {

TilesetImageLabel::TilesetImageLabel(const QImage& image,
                                     int tileWidth,
                                     int tileHeight,
                                     QWidget* parent)
    : QLabel{parent},
      m_tileWidth{tileWidth < 1 ? 1 : tileWidth},
      m_tileHeight{tileHeight < 1 ? 1 : tileHeight}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tileset image label from null image!"};
  }
  setPixmap(QPixmap::fromImage(image));

  m_width = image.width();
  m_height = image.height();

  m_maxX = m_width - 1;
  m_maxY = m_height - 1;

  m_nRows = m_height / m_tileWidth;
  m_nCols = m_width / m_tileHeight;
}

TilesetImageLabel::~TilesetImageLabel() noexcept
{}

void TilesetImageLabel::paintEvent(QPaintEvent* event)
{
  QLabel::paintEvent(event);

  QPainter painter{this};

  for (auto row = 0; row < m_nRows; ++row) {
    for (auto col = 0; col < m_nCols; ++col) {
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

}  // namespace tactile
