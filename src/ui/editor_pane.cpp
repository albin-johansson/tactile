#include "editor_pane.h"

#include <QOpenGLFunctions>
#include <QPainter>
#include <QResizeEvent>
#include <QSurfaceFormat>

namespace tactile {

EditorPane::EditorPane(QWidget* parent) noexcept
    : QWidget{parent}, m_viewport{0, 0, width(), height()}
{
  QSizePolicy policy;
  policy.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
  policy.setVerticalPolicy(QSizePolicy::Policy::Expanding);
  setSizePolicy(policy);
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

  m_viewport.setWidth(event->size().width());
  m_viewport.setHeight(event->size().height());
}

}  // namespace tactile
