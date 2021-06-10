#include "tileset_image_widget.hpp"

#include <QGridLayout>
#include <QMouseEvent>

#include "tileset_image_label.hpp"
#include "tileset_rubber_band.hpp"

namespace tactile {

TilesetImageWidget::TilesetImageWidget(const core::Tileset& tileset,
                                       QWidget* parent)
    : QWidget{parent}
    , mLayout{new QGridLayout{this}}
{
  mLayout->setContentsMargins(0, 0, 0, 0);

  mImageLabel = new TilesetImageLabel{tileset, this};
  mRubberBand = new TilesetRubberBand{mImageLabel};
  mRubberBand->SetTileWidth(tileset.GetTileWidth());
  mRubberBand->SetTileHeight(tileset.GetTileHeight());

  connect(mRubberBand,
          &TilesetRubberBand::S_FinishedSelection,
          this,
          &TilesetImageWidget::S_SetTilesetSelection);

  mLayout->addWidget(mImageLabel);
  setLayout(mLayout);
}

TilesetImageWidget::~TilesetImageWidget() noexcept = default;

void TilesetImageWidget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
  if (event->buttons() & Qt::MouseButton::LeftButton)
  {
    event->accept();
    mRubberBand->MousePressed(event->pos());
  }
}

void TilesetImageWidget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
  if (event->buttons() & Qt::MouseButton::LeftButton)
  {
    event->accept();
    mRubberBand->MouseMoved(event->pos());
  }
}

void TilesetImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  if (event->button() == Qt::MouseButton::LeftButton)
  {
    event->accept();
    mRubberBand->MouseReleased();
  }
}

}  // namespace tactile
