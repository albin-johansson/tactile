#include "tilemap_item.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "render_tilemap.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

tilemap_item::tilemap_item(model::core_model* core, QGraphicsItem* parent)
    : QGraphicsItem{parent}
{}

void tilemap_item::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget*)
{
  assert(painter);
  assert(option);

  // TODO store pointer to associated tilemap
  emit request_redraw(*painter, option->exposedRect);
}

auto tilemap_item::boundingRect() const -> QRectF
{
  // FIXME
  return QRectF();
}

}  // namespace tactile::gui
