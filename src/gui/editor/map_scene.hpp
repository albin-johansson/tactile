#pragma once

#include <QGraphicsScene>
#include <QRect>
#include <QWidget>

#include "core_model.hpp"

class QPainter;

namespace tactile::gui {

class map_scene final : public QGraphicsScene
{
  Q_OBJECT

 public:
  explicit map_scene(not_null<model::tilemap*> map,
                     map_id id,
                     QWidget* parent = nullptr);

  [[nodiscard]] auto id() const noexcept -> map_id;

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

 private:
  //  QRect m_viewport{};
  map_id m_id;
  QPointF m_lastMouseScenePos{};
};

}  // namespace tactile::gui
