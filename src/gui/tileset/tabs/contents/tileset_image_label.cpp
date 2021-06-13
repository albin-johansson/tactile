#include "tileset_image_label.hpp"

#include <QPainter>

namespace tactile {

TilesetImageLabel::TilesetImageLabel(const core::Tileset& tileset,
                                     QWidget* parent)
    : QLabel{parent}
    , mTileWidth{tileset.GetTileWidth()}
    , mTileHeight{tileset.GetTileHeight()}
    , mWidth{tileset.Width()}
    , mHeight{tileset.Height()}
    , mMaxX{mWidth - 1}
    , mMaxY{mHeight - 1}
{
  setObjectName(QStringLiteral(u"TilesetImageLabel"));
  setPixmap(tileset.Image());
}

TilesetImageLabel::~TilesetImageLabel() noexcept = default;

void TilesetImageLabel::paintEvent(QPaintEvent* event)
{
  QLabel::paintEvent(event);

  QPainter painter{this};
  painter.setPen(QPen{Qt::black, 1});

  const auto region = visibleRegion().boundingRect();

  const auto tileWidth = mTileWidth.get();
  const auto tileHeight = mTileHeight.get();

  const auto minRow = region.y() / tileHeight;
  const auto minCol = region.x() / tileWidth;
  const auto maxRow = ((region.y() + region.height()) / tileHeight) + 1;
  const auto maxCol = ((region.x() + region.width()) / tileWidth) + 1;

  for (auto row = minRow; row < maxRow; ++row)
  {
    for (auto col = minCol; col < maxCol; ++col)
    {
      const auto x = col * tileWidth;
      const auto y = row * tileHeight;
      painter.drawRect(x, y, mWidth, mHeight);
    }
  }

  {
    // renders the bottom and right-hand end lines
    painter.drawLine(0, mMaxY, mWidth, mMaxY);
    painter.drawLine(mMaxX, 0, mMaxX, mHeight);
  }
}

}  // namespace tactile
