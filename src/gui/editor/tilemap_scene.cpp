#include "tilemap_scene.hpp"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QResizeEvent>

#include "map_item.hpp"

namespace tactile::gui {

tilemap_scene::tilemap_scene(not_null<model::tilemap*> map, int id, QWidget* parent)
    : QGraphicsScene{parent}, m_id{id}
{
  auto* item = new map_item{map};
//  connect(item,
//          &tilemap_item::request_redraw,
//          this,
//          &tilemap_scene::request_redraw);
  addItem(item);

  setSceneRect(100, 100, 100, 100);
}

// void tilemap_scene::center_viewport(int mapWidth, int mapHeight) noexcept
//{
//  const auto x = (m_viewport.width() - mapWidth) / 2;
//  const auto y = (m_viewport.height() - mapHeight) / 2;
//
//  m_viewport.moveTo(x, y);
//}

// void tilemap_scene::move_viewport(int dx, int dy) noexcept
//{
//  const auto width = m_viewport.width();
//  const auto height = m_viewport.height();
//
//  m_viewport.translate(dx, dy);
//
//  m_viewport.setWidth(width);
//  m_viewport.setHeight(height);
//}

auto tilemap_scene::id() const noexcept -> int
{
  return m_id;
}

// void tilemap_scene::drawBackground(QPainter* painter, const QRectF& rect)
//{
//  QGraphicsScene::drawBackground(painter, rect);
//  painter->fillRect(rect, Qt::black);
//}
//
// void tilemap_scene::drawForeground(QPainter* painter, const QRectF& rect)
//{
//  QGraphicsScene::drawForeground(painter, rect);
//
//  //  m_viewport.setSize(rect.size().toSize());
//
//  //  painter->setPen(Qt::magenta);
//  //  painter->drawRect();
//
//  emit request_redraw(*painter);
//}

// void tilemap_scene::resizeEvent(QResizeEvent* event)
//{
//  QWidget::resizeEvent(event);
//
//  const auto& newSize = event->size();
//  const auto newWidth = newSize.width();
//  const auto newHeight = newSize.height();
//
//  m_viewport.setWidth(newWidth);
//  m_viewport.setHeight(newHeight);
//
//  update();
//}

void tilemap_scene::drawBackground(QPainter* painter, const QRectF& rect)
{
  QGraphicsScene::drawBackground(painter, rect);
  painter->fillRect(rect, Qt::black);
}

void tilemap_scene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsScene::mousePressEvent(event);

  //  if (event->buttons() & Qt::MouseButton::MidButton) {
  //    const auto pos = event->scenePos();
  //
  //    m_lastMouseX = pos.x();
  //    m_lastMouseY = pos.y();
  //
  //    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  //  }
}

void tilemap_scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsScene::mouseReleaseEvent(event);
  //
  //  QApplication::restoreOverrideCursor();
}

void tilemap_scene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsScene::mouseMoveEvent(event);
  m_lastMouseScenePos = event->scenePos();
}

}  // namespace tactile::gui
