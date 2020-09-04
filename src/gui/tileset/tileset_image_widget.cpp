#include "tileset_image_widget.hpp"

#include <qapplication.h>
#include <qevent.h>
#include <qgridlayout.h>
#include <qpainter.h>

#include <algorithm>  // max

#include "tactile_error.hpp"
#include "tileset_image_label.hpp"

namespace tactile::gui {

tileset_image_widget::tileset_image_widget(const QImage& image,
                                           tile_width tileWidth,
                                           tile_height tileHeight,
                                           QWidget* parent)
    : QWidget{parent}, m_tileWidth{tileWidth}, m_tileHeight{tileHeight}
{
  if (image.isNull()) {
    throw tactile_error{"Can't create tileset image widget from null image!"};
  }

  m_layout = new QGridLayout{this};
  m_imageLabel = new tileset_image_label{image, tileWidth, tileHeight, this};
  m_rubberBand = new QRubberBand{QRubberBand::Rectangle, m_imageLabel};

  m_layout->addWidget(m_imageLabel);
  setLayout(m_layout);
}

tileset_image_widget::~tileset_image_widget() noexcept = default;

auto tileset_image_widget::get_adjusted_selection() const -> QRect
{
  const auto& geometry = m_rubberBand->geometry();

  const auto gX = geometry.x();
  const auto gY = geometry.y();
  const auto gRight = geometry.right();
  const auto gBottom = geometry.bottom();

  const auto tileWidth = m_tileWidth.get();
  const auto tileHeight = m_tileHeight.get();

  QRect newGeometry;
  newGeometry.setX(gX - (gX % tileWidth));
  newGeometry.setY(gY - (gY % tileHeight));

  const auto modRight = gRight % tileWidth;
  newGeometry.setRight(gRight - modRight);

  const auto modBottom = gBottom % tileHeight;
  newGeometry.setBottom(gBottom - modBottom);

  // TODO Improve handling of 1 by X and X by 1 selections

  //  Q_ASSERT(newGeometry.x() % tileWidth == 0);
  //  Q_ASSERT(newGeometry.y() % tileHeight == 0);
  //  Q_ASSERT(newGeometry.width() % tileWidth == 0);
  //  Q_ASSERT(newGeometry.height() % tileHeight == 0);

  return newGeometry;
}

void tileset_image_widget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  m_lastMousePos = event->pos();

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_origin = m_lastMousePos;

    const QPoint bottomRight{m_origin.x() + m_tileWidth.get(),
                             m_origin.y() + m_tileHeight.get()};

    m_rubberBand->setGeometry(QRect{m_origin, bottomRight});
    m_rubberBand->setGeometry(get_adjusted_selection());
    m_rubberBand->show();
  }
}

void tileset_image_widget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  const auto pos = event->pos();

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_rubberBand->setGeometry(QRect{m_origin, pos}.normalized());
    m_rubberBand->setGeometry(get_adjusted_selection());
  }

  m_lastMousePos = pos;
}

void tileset_image_widget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  if (event->button() == Qt::MouseButton::LeftButton) {
    const auto selection = get_adjusted_selection();

    if (selection.width() >= m_tileWidth.get() &&
        selection.height() >= m_tileHeight.get()) {
      m_rubberBand->setGeometry(selection);

      // TODO apply selection to model

    } else {
      m_rubberBand->hide();  // selection was too small, just hide it
    }
  }
}

}  // namespace tactile::gui
