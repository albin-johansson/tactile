#include "tileset_image_widget.hpp"

#include <QGridLayout>
#include <QMouseEvent>

#include "tileset_image_label.hpp"
#include "tileset_rubber_band.hpp"

namespace tactile::gui {

tileset_image_widget::tileset_image_widget(const core::tileset& tileset,
                                           QWidget* parent)
    : QWidget{parent}
{
  m_layout = new QGridLayout{this};
  m_layout->setContentsMargins(0, 0, 0, 0);

  m_imageLabel = new tileset_image_label{tileset, this};

  m_rubberBand = new tileset_rubber_band{m_imageLabel};
  m_rubberBand->set_tile_width(tileset.get_tile_width());
  m_rubberBand->set_tile_height(tileset.get_tile_height());
  connect(m_rubberBand,
          &tileset_rubber_band::finished_selection,
          this,
          &tileset_image_widget::ui_set_tileset_selection);

  m_layout->addWidget(m_imageLabel);
  setLayout(m_layout);
}

tileset_image_widget::~tileset_image_widget() noexcept = default;

void tileset_image_widget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
  if (event->buttons() & Qt::MouseButton::LeftButton)
  {
    event->accept();
    m_rubberBand->mouse_pressed(event->pos());
  }
}

void tileset_image_widget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
  if (event->buttons() & Qt::MouseButton::LeftButton)
  {
    event->accept();
    m_rubberBand->mouse_moved(event->pos());
  }
}

void tileset_image_widget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  if (event->button() == Qt::MouseButton::LeftButton)
  {
    event->accept();
    m_rubberBand->mouse_released();
  }
}

}  // namespace tactile::gui
