#include "map_view.hpp"

#include <QApplication>
#include <QScrollBar>

#include "map_document.hpp"
#include "map_scene.hpp"

namespace tactile::gui {

map_view::map_view(core::map_document* map, const map_id id, QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  setAttribute(Qt::WA_StaticContents, true);

  // register mouse events with no pressed buttons
  setMouseTracking(true);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  //  grabGesture(Qt::PinchGesture);

  setScene(new map_scene{map, id, this});
}

void map_view::force_redraw()
{
  scene()->update();
}

void map_view::center_map()
{
  if (auto* scene = get_map_scene()) {
    scene->center_map();
  }
}

void map_view::move_map(const int dx, const int dy)
{
  if (auto* scene = get_map_scene()) {
    scene->move_map(dx, dy);
  }
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

void map_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);

  if (event->buttons() & Qt::MouseButton::MidButton) {
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

  if (event->button() == Qt::MouseButton::MidButton) {
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

void map_view::wheelEvent(QWheelEvent* event)
{
  if (event->modifiers() & Qt::ControlModifier) {
    auto pixels = event->pixelDelta();
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

  }
}

auto map_view::get_map_scene() -> map_scene*
{
  return qobject_cast<map_scene*>(scene());
}

auto map_view::get_map_scene() const -> const map_scene*
{
  return qobject_cast<const map_scene*>(scene());
}

}  // namespace tactile::gui
