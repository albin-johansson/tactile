#pragma once

#include <QGraphicsItem>
#include <QObject>

#include "core.hpp"

namespace tactile::ui {

class tilemap_item final : public QObject, public QGraphicsItem  {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

 public:
  explicit tilemap_item(model::core* core, QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 signals:
  void request_redraw(QPainter& painter, const QRectF& exposed);
};

}  // namespace tactile::ui
