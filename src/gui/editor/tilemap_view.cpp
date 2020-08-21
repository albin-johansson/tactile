#include "tilemap_view.hpp"

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScrollBar>

namespace tactile::gui {

tilemap_view::tilemap_view(not_null<model::tilemap*> map,
                           map_id id,
                           QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  setScene(new map_scene{map, id, this});
}

void tilemap_view::move_viewport(int dx, int dy) noexcept
{
  const auto old = scene()->sceneRect();
  scene()->setSceneRect(old.x() + dx, old.y() + dy, old.width(), old.height());
}

void tilemap_view::center_viewport(int mapWidth, int mapHeight) noexcept
{
  // TODO
  //  m_scene->center_viewport(mapWidth, mapHeight);
}

void tilemap_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);
}

void tilemap_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  qInfo("Mouse move event!");

  m_lastMousePos = event->globalPos();
  m_lastMouseScenePos = mapToScene(viewport()->mapFromGlobal(m_lastMousePos));

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

void tilemap_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

auto tilemap_view::id() const noexcept -> map_id
{
  return get_scene()->id();
}

void tilemap_view::force_redraw()
{
  scene()->update(0, 0, 1'000, 1'000);  // FIXME
}

auto tilemap_view::get_scene() const noexcept -> const map_scene*
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
