#include "tileset_tab.hpp"

#include <qapplication.h>
#include <qevent.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qscrollarea.h>
#include <qscrollbar.h>

#include "tactile_error.hpp"
#include "tileset_image_widget.hpp"

namespace tactile::gui {

tileset_tab::tileset_tab(const QImage& image,
                         tileset_id id,
                         tile_width tileWidth,
                         tile_height tileHeight,
                         QWidget* parent)
    : QWidget{parent},
      m_id{id}
{
  if (image.isNull()) {
    throw tactile_error{"Can't create tileset tab from null image!"};
  }

  setObjectName("tileset_tab");

  m_layout = new QGridLayout{this};
  m_layout->setMargin(0);

  m_scrollArea = new QScrollArea{this};
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);

  m_imageWidget = new tileset_image_widget{image, tileWidth, tileHeight, this};
  m_scrollArea->setWidget(m_imageWidget);

  m_layout->addWidget(m_scrollArea);

  setLayout(m_layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(m_imageWidget,
          &tileset_image_widget::tileset_selection_changed,
          this,
          &tileset_tab::tileset_selection_changed);
}

tileset_tab::~tileset_tab() noexcept = default;

void tileset_tab::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  m_lastMousePos = event->pos();

  if (event->buttons() & Qt::MouseButton::MidButton) {
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }
}

void tileset_tab::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  const auto pos = event->pos();

  if (event->buttons() & Qt::MouseButton::MidButton) {
    auto* vbar = m_scrollArea->verticalScrollBar();
    auto* hbar = m_scrollArea->horizontalScrollBar();
    hbar->setValue(hbar->value() - pos.x() + m_lastMousePos.x());
    vbar->setValue(vbar->value() - pos.y() + m_lastMousePos.y());
  }

  m_lastMousePos = pos;
}

void tileset_tab::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  QApplication::restoreOverrideCursor();
}

}  // namespace tactile::gui
