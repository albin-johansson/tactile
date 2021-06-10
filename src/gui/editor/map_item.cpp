#include "map_item.hpp"

#include <QStyleOption>  // QStyleOption

#include "map.hpp"
#include "map_document.hpp"
#include "position.hpp"
#include "render_map.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile {

MapItem::MapItem(core::MapDocument* map, QGraphicsItem* parent)
    : QGraphicsItem{parent}
    , mDocument{map}
{
  if (!mDocument)
  {
    throw tactile_error{"Cannot create map item based on null map document!"};
  }
  // This is to be able to use exposedRect
  setFlags(QGraphicsItem::ItemUsesExtendedStyleOption);
}

void MapItem::DisableStampPreview()
{
  mMousePosition.reset();
  update();
}

void MapItem::EnableStampPreview(const core::position& position)
{
  mMousePosition = position;
}

void MapItem::ShowMapProperties()
{
  emit mDocument->S_ShowProperties();
}

void MapItem::paint(QPainter* painter,
                    const QStyleOptionGraphicsItem* option,
                    QWidget*)
{
  Q_ASSERT(painter);
  Q_ASSERT(option);
  Q_ASSERT(mDocument);

  RenderMap(*painter, *mDocument, mMousePosition, option->exposedRect);

  painter->setPen(Qt::red);
  painter->setPen(Qt::DashLine);
  painter->drawRect(boundingRect());
}

auto MapItem::boundingRect() const -> QRectF
{
  const auto width = static_cast<double>(mDocument->Width());
  const auto height = static_cast<double>(mDocument->Height());
  return {0, 0, width, height};
}

}  // namespace tactile
