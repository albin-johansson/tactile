#include "tileset_tab.hpp"

#include <QApplication>
#include <QGridLayout>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

#include "tileset_image_widget.hpp"

namespace tactile::gui {

tileset_tab::tileset_tab(tileset_id id,
                         const core::tileset& tileset,
                         QWidget* parent)
    : QWidget{parent},
      m_id{id},
      m_name{tileset.name()}
{
  setObjectName("tileset_tab");

  m_layout = new QGridLayout{this};
  m_layout->setContentsMargins(0, 0, 0, 0);

  m_scrollArea = new QScrollArea{this};
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);

  m_imageWidget = new tileset_image_widget{tileset, this};
  m_scrollArea->setWidget(m_imageWidget);

  m_layout->addWidget(m_scrollArea);

  setLayout(m_layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(m_imageWidget,
          &tileset_image_widget::ui_set_tileset_selection,
          this,
          &tileset_tab::set_tileset_selection);
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
