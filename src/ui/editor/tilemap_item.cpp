#include "tilemap_item.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "render_tilemap.hpp"
#include "tactile_error.hpp"

namespace tactile::ui {

tilemap_item::tilemap_item(model::core* core, QGraphicsItem* parent)
    : QGraphicsItem{parent}
{}

void tilemap_item::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget*)
{
  assert(painter);
  assert(option);

//  const auto viewport = painter->viewport();
//  qInfo("Viewport: (%i, %i, %i, %i)",
//        viewport.x(),
//        viewport.y(),
//        viewport.width(),
//        viewport.height());

  emit request_redraw(*painter, option->exposedRect);
}

auto tilemap_item::boundingRect() const -> QRectF
{
  // FIXME
  return QRectF();
}

}  // namespace tactile::ui
