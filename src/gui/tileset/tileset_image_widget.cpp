#include "tileset_image_widget.hpp"

#include <qapplication.h>
#include <qevent.h>
#include <qgridlayout.h>
#include <qpainter.h>

#include "tactile_error.hpp"
#include "tileset_image_label.hpp"

namespace tactile::gui {

tileset_image_widget::tileset_image_widget(const QImage& image,
                                           tile_width tileWidth,
                                           tile_height tileHeight,
                                           QWidget* parent)
    : QWidget{parent}
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

void tileset_image_widget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  m_lastMousePos = event->pos();

  const auto buttons = event->buttons();
  if (buttons & Qt::MouseButton::LeftButton) {
    m_origin = m_lastMousePos;
    m_rubberBand->setGeometry(QRect{m_origin, QSize{}});
    m_rubberBand->show();
  }
}

void tileset_image_widget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  const auto buttons = event->buttons();
  const auto pos = event->pos();

  if (buttons & Qt::MouseButton::LeftButton) {
    m_rubberBand->setGeometry(QRect{m_origin, pos}.normalized());
  }

  m_lastMousePos = pos;
}

void tileset_image_widget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
}

}  // namespace tactile::gui
