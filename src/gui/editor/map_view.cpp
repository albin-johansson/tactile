#include "map_view.hpp"

#include <qdebug.h>
#include <qevent.h>
#include <qscrollbar.h>

namespace tactile::gui {

map_view::map_view(not_null<model::tilemap*> map, map_id id, QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);

  // TODO define range
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  setScene(new map_scene{map, id, this});
}

void map_view::move_map(int dx, int dy)
{
  get_map_scene()->move_map_item(dx, dy);
}

void map_view::center_map()
{
  // TODO reimplement
  //  m_scene->center_viewport(mapWidth, mapHeight);
}

void map_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);
}

void map_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  //  m_lastMousePos = event->globalPos();
  //  m_lastMouseScenePos =
  //  mapToScene(viewport()->mapFromGlobal(m_lastMousePos));

  //    if (event->buttons() & Qt::MouseButton::MidButton) {
  //      const auto pos = event->pos();
  //
  //      const int x = pos.x();
  //      const int y = pos.y();
  //
  //      translate(x - m_lastMouseX, y - m_lastMouseY);
  //
  //      m_lastMouseX = x;
  //      m_lastMouseY = y;
  //      update();
  //    }
}

void map_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

auto map_view::id() const -> map_id
{
  return get_map_scene()->id();
}

void map_view::force_redraw()  // TODO remove?!!
{
  scene()->update();
}

auto map_view::get_map_scene() -> map_scene*
{
  return qobject_cast<map_scene*>(scene());
}

auto map_view::get_map_scene() const -> const map_scene*
{
  return qobject_cast<const map_scene*>(scene());
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
