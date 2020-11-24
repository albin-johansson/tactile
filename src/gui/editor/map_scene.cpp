#include "map_scene.hpp"

#include <QApplication>
#include <QGraphicsSceneEvent>
#include <QPainter>

#include "map_item.hpp"

namespace tactile::gui {

map_scene::map_scene(core::map_document* map, const map_id id, QObject* parent)
    : QGraphicsScene{parent},
      m_item{new map_item{map}},
      m_id{id}
{
  addItem(m_item);
  setActivePanel(m_item);
}

void map_scene::move_map(const int dx, const int dy)
{
  m_item->moveBy(dx, dy);
}

void map_scene::center_map()
{
  const auto rect = sceneRect();
  const auto bounds = m_item->boundingRect();

  m_item->setX(rect.x() + (rect.width() - bounds.width()) / 2.0);
  m_item->setY(rect.y() + (rect.height() - bounds.height()) / 2.0);
}

void map_scene::enable_stamp_preview(const core::position& position)
{
  m_item->enable_stamp_preview(position);
}

void map_scene::disable_stamp_preview()
{
  m_item->disable_stamp_preview();
}

auto map_scene::map_position() const -> QPointF
{
  return m_item->pos();
}

void map_scene::drawBackground(QPainter* painter, const QRectF& rect)
{
  QGraphicsScene::drawBackground(painter, rect);
  painter->fillRect(rect, Qt::black);
}

}  // namespace tactile::gui
