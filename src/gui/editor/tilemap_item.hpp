#pragma once

#include <QGraphicsItem>
#include <QObject>

#include "core_model.hpp"

namespace tactile::gui {

// TODO avoid QObject inheritance
class tilemap_item final : public QGraphicsItem
{
 public:
  explicit tilemap_item(not_null<model::tilemap*> map,
                        QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  model::tilemap* m_map;
};

}  // namespace tactile::gui
