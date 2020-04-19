#include "tileset_image_widget.h"

#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

#include "tactile_types.h"
#include "tileset_image_label.h"

namespace tactile {

TilesetImageWidget::TilesetImageWidget(const QImage& image,
                                       int tileWidth,
                                       int tileHeight,
                                       QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tileset image widget from null image!"};
  }

  m_imageLabel = std::make_unique<TilesetImageLabel>(
      image, tileWidth, tileHeight);

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_imageLabel.get());

  setLayout(m_layout.get());

  m_rubberBand =
      std::make_unique<QRubberBand>(QRubberBand::Rectangle, m_imageLabel.get());
}

TilesetImageWidget::~TilesetImageWidget() noexcept = default;

void TilesetImageWidget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  if (event->buttons() & Qt::MouseButton::LeftButton &&
      !m_rubberBand->isVisible()) {
    m_origin = event->pos();

    m_rubberBand->setGeometry(QRect{m_origin, QSize()});
    m_rubberBand->show();
  }
}

void TilesetImageWidget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_rubberBand->setGeometry(QRect{m_origin, event->pos()}.normalized());
  }
}

void TilesetImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  if (event->button() == Qt::MouseButton::LeftButton) {
    m_rubberBand->hide();
  }
  // TODO compute the selection
}

}  // namespace tactile
