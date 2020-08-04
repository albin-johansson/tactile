#include "tilemap_view.hpp"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>

namespace tactile::ui {

tilemap_view::tilemap_view(int id, QWidget* parent) : QGraphicsView{parent}
{
  setScene(new tilemap_scene{id, this});

  connect(get_tilemap_scene(),
          &tilemap_scene::request_redraw,
          this,
          &tilemap_view::request_redraw);

}

void tilemap_view::move_viewport(int dx, int dy) noexcept
{
  translate(dx, dy);
}

void tilemap_view::center_viewport(int mapWidth, int mapHeight) noexcept
{
  //  m_scene->center_viewport(mapWidth, mapHeight);
}

void tilemap_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);
}

void tilemap_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  m_lastMousePos = event->globalPos();
  m_lastMouseScenePos = mapToScene(viewport()->mapFromGlobal(m_lastMousePos));

  //  if (event->buttons() & Qt::MouseButton::MidButton) {
  //    const auto pos = event->pos();
  //
  //    const int x = pos.x();
  //    const int y = pos.y();
  //
  //    translate(x - m_lastMouseX, y - m_lastMouseY);
  //
  //    m_lastMouseX = x;
  //    m_lastMouseY = y;
  //    update();
  //  }
}

void tilemap_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

// void tilemap_view::paintEvent(QPaintEvent* event)
//{
//  QGraphicsView::paintEvent(event);
//}
//
void tilemap_view::resizeEvent(QResizeEvent* event)
{
  QGraphicsView::resizeEvent(event);

  //  scene()->setSceneRect(0, 0, event->size().width(),
  //  event->size().height());
}

auto tilemap_view::id() const noexcept -> int
{
  return get_scene()->id();
}

auto tilemap_view::get_tilemap_scene() noexcept -> tilemap_scene*
{
  return qobject_cast<tilemap_scene*>(scene());
}

auto tilemap_view::get_scene() const noexcept -> const tilemap_scene*
{
  return qobject_cast<const tilemap_scene*>(scene());
}

}  // namespace tactile::ui
