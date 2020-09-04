#include "map_view.hpp"

#include <qdebug.h>
#include <qevent.h>
#include <qscrollbar.h>

namespace tactile::gui {

map_view::map_view(not_null<core::map*> map, map_id id, QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);

  // TODO define range
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

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

  m_lastMousePos = event->pos();
}

void map_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  const auto pos = event->pos();

  if (event->buttons() & Qt::MouseButton::MidButton) {
    move_map(pos.x() - m_lastMousePos.x(), pos.y() - m_lastMousePos.y());
  }

  m_lastMousePos = pos;
}

void map_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

// bool tilemap_view::event(QEvent* event)
//{
//  if (const auto type = event->type();
//      type == QEvent::TouchBegin || type == QEvent::TouchUpdate ||
//      type == QEvent::TouchEnd || type == QEvent::TouchCancel) {
//    qInfo("Touch!");
//    return QGraphicsView::event(event);
//  } else {
//    return QGraphicsView::event(event);
//  }
//}

}  // namespace tactile::gui
