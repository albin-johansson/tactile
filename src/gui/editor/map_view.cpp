#include "map_view.hpp"

#include <qapplication.h>
#include <qdebug.h>
#include <qevent.h>
#include <qscrollbar.h>

namespace tactile::gui {

map_view::map_view(core::map_document* map, map_id id, QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);

  // TODO define range
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setMouseTracking(true);  // register mouse events with no pressed buttons

  setScene(new map_scene{map, id, this});
}

void map_view::force_redraw()
{
  scene()->update();
}

void map_view::center_map()
{
  get_map_scene()->center_map();
}

void map_view::move_map(int dx, int dy)
{
  get_map_scene()->move_map(dx, dy);
}

void map_view::enable_stamp_preview(const core::position& position)
{
  get_map_scene()->enable_stamp_preview(position);
}

void map_view::disable_stamp_preview()
{
  get_map_scene()->disable_stamp_preview();
}

auto map_view::id() const -> map_id
{
  return get_map_scene()->id();
}

auto map_view::get_map_scene() -> map_scene*
{
  return qobject_cast<map_scene*>(scene());
}

auto map_view::get_map_scene() const -> const map_scene*
{
  return qobject_cast<const map_scene*>(scene());
}

void map_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);

  const auto buttons = event->buttons();

  if (buttons & Qt::MouseButton::MidButton) {
    m_lastMousePos = event->pos();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }

  emit mouse_pressed(event, mapFromScene(get_map_scene()->map_position()));
}

void map_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  const auto pos = event->pos();
  const auto buttons = event->buttons();

  if (buttons & Qt::MouseButton::MidButton) {
    move_map(pos.x() - m_lastMousePos.x(), pos.y() - m_lastMousePos.y());
  }

  m_lastMousePos = pos;

  emit mouse_moved(event, mapFromScene(get_map_scene()->map_position()));
}

void map_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);

  const auto button = event->button();

  if (button == Qt::MouseButton::MidButton) {
    QApplication::restoreOverrideCursor();
  }

  emit mouse_released(event, mapFromScene(get_map_scene()->map_position()));
}

void map_view::enterEvent(QEvent* event)
{
  QWidget::enterEvent(event);
  emit mouse_entered(event);
}

void map_view::leaveEvent(QEvent* event)
{
  QWidget::leaveEvent(event);
  emit mouse_exited(event);
}

void map_view::keyPressEvent(QKeyEvent* event)
{
  QGraphicsView::keyPressEvent(event);

  if (event->key() == Qt::Key::Key_Control) {
    m_canZoom = true;
  }
}

void map_view::keyReleaseEvent(QKeyEvent* event)
{
  QGraphicsView::keyReleaseEvent(event);

  if (event->key() == Qt::Key::Key_Control) {
    m_canZoom = false;
  }
}

void map_view::wheelEvent(QWheelEvent* event)
{
  QGraphicsView::wheelEvent(event);

  if (m_canZoom) {
    const auto pixels = event->pixelDelta();
    const auto degrees = event->angleDelta() / 8;

    if (!pixels.isNull()) {
      if (pixels.y() > 0) {
        emit increase_zoom();
      } else {
        emit decrease_zoom();
      }
    } else if (!degrees.isNull()) {
      const auto numSteps = degrees / 15;
      if (numSteps.y() > 0) {
        emit increase_zoom();
      } else {
        emit decrease_zoom();
      }
    }

    event->accept();
  }
}

}  // namespace tactile::gui
