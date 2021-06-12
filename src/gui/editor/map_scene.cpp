#include "map_scene.hpp"

#include <QApplication>
#include <QGraphicsSceneEvent>
#include <QPainter>

#include "map_item.hpp"

namespace tactile {

MapScene::MapScene(core::MapDocument* map, const map_id id, QObject* parent)
    : QGraphicsScene{parent}
    , mItem{new MapItem{map}}
    , mId{id}
{
  addItem(mItem);
}

void MapScene::MoveViewport(const int dx, const int dy)
{
  mItem->moveBy(dx, dy);
}

void MapScene::EnableStampPreview(const core::MapPosition& position)
{
  mItem->EnableStampPreview(position);
}

void MapScene::DisableStampPreview()
{
  mItem->DisableStampPreview();
}

void MapScene::ShowMapProperties()
{
  mItem->ShowMapProperties();
}

void MapScene::ResetScale()
{
  mItem->setScale(1.0);
}

void MapScene::IncreaseScale()
{
  mItem->setScale(mItem->scale() * 1.1);
}

void MapScene::DecreaseScale()
{
  mItem->setScale(mItem->scale() * 0.9);
}

void MapScene::SetScale(qreal scale)
{
  mItem->setScale(scale);
}

auto MapScene::CurrentMapPosition() const -> QPointF
{
  return mItem->pos();
}

auto MapScene::MapBounds() const -> QRectF
{
  return mItem->boundingRect();
}

void MapScene::drawBackground(QPainter* painter, const QRectF& rect)
{
  QGraphicsScene::drawBackground(painter, rect);

  // TODO introduce setting for the background color
  painter->fillRect(rect, QColor{Qt::darkGray}.darker());
}

}  // namespace tactile
