#include "editor_tab.hpp"

#include <QApplication>
#include <QPainter>
#include <QResizeEvent>

#include "widget_size_policy.hpp"

namespace tactile {

EditorTab::EditorTab(int id, QWidget* parent)
    : QWidget{parent},
      m_viewport{0, 0, width(), height()},
      m_id{id},
      m_lastMouseX{0},
      m_lastMouseY{0}
{
  set_size_policy(this, QSizePolicy::Policy::Expanding);
}

void EditorTab::center_viewport(int mapWidth, int mapHeight) noexcept
{
  const auto x = (m_viewport.width() - mapWidth) / 2;
  const auto y = (m_viewport.height() - mapHeight) / 2;

  m_viewport.moveTo(x, y);
}

void EditorTab::move_viewport(int dx, int dy) noexcept
{
  const auto width = m_viewport.width();
  const auto height = m_viewport.height();

  m_viewport.translate(dx, dy);
  m_viewport.setWidth(width);
  m_viewport.setHeight(height);
}

void EditorTab::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  QPainter painter{this};
  painter.fillRect(0, 0, width(), height(), Qt::black);
  painter.setViewport(m_viewport);

  emit s_redraw(painter);
}

void EditorTab::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);

  const auto& newSize = event->size();
  const auto newWidth = newSize.width();
  const auto newHeight = newSize.height();

  m_viewport.setWidth(newWidth);
  m_viewport.setHeight(newHeight);

  update();
}

void EditorTab::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
  if (event->buttons() & Qt::MouseButton::MidButton) {
    m_lastMouseX = event->x();
    m_lastMouseY = event->y();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }
}

void EditorTab::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  QApplication::restoreOverrideCursor();
}

void EditorTab::mouseMoveEvent(QMouseEvent* event)
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
