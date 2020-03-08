#include "editor_pane.h"

#include <QApplication>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QResizeEvent>
#include <QSurfaceFormat>

namespace tactile {

EditorPane::EditorPane(QWidget* parent) noexcept
    : QWidget{parent},
      m_viewport{0, 0, width(), height()},
      m_lastMouseX{0},
      m_lastMouseY{0}
{
  QSizePolicy policy;
  policy.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
  policy.setVerticalPolicy(QSizePolicy::Policy::Expanding);
  setSizePolicy(policy);

//  setAttribute(Qt::WA_AcceptTouchEvents);
}

void EditorPane::move_viewport(int dx, int dy) noexcept
{
  m_viewport.translate(dx, dy);
  m_viewport.setWidth(width());
  m_viewport.setHeight(height());
}

void EditorPane::set_viewport_pos(int x, int y) noexcept
{
  m_viewport.moveTo(x, y);
}

void EditorPane::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  emit req_redraw();
}

void EditorPane::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);

  const auto& size = event->size();
  m_viewport.setWidth(size.width());
  m_viewport.setHeight(size.height());
}

void EditorPane::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  if (event->buttons() & Qt::MouseButton::MidButton) {
    m_lastMouseX = event->x();
    m_lastMouseY = event->y();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }
}

void EditorPane::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  QApplication::restoreOverrideCursor();
}

void EditorPane::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  if (event->buttons() & Qt::MouseButton::MidButton) {
    m_viewport.translate(event->x() - m_lastMouseX, event->y() - m_lastMouseY);
    m_lastMouseX = event->x();
    m_lastMouseY = event->y();
    update();
  }
}

}  // namespace tactile
