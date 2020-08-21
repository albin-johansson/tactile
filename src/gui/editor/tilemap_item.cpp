#include "tilemap_item.hpp"

#include <QGraphicsWidget>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "render_tilemap.hpp"

namespace tactile::gui {

tilemap_item::tilemap_item(not_null<model::tilemap*> map, QGraphicsItem* parent)
    : QGraphicsItem{parent}, m_map{map}
{}

void tilemap_item::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget*)
{
  Q_ASSERT(painter);
  Q_ASSERT(option);
  model::render_tilemap(*painter, *m_map, option->exposedRect);
}

auto tilemap_item::boundingRect() const -> QRectF
{
  const qreal width = m_map->width();
  const qreal height = m_map->height();
  return {0, 0, width, height};
}

}  // namespace tactile::gui
