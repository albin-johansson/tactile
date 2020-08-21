#pragma once

#include <QGraphicsItem>
#include <QObject>

#include "core_model.hpp"

namespace tactile::gui {

class map_item final : public QGraphicsItem
{
 public:
  explicit map_item(not_null<model::tilemap*> map,
                    QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  model::tilemap* m_map;

  void draw_layer(QPainter& painter,
                  const model::tile_layer& layer,
                  const QRectF& exposed);
};

}  // namespace tactile::gui
