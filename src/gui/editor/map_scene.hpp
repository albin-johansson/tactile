#pragma once

#include <qgraphicsscene.h>
#include <qrect.h>
#include <qwidget.h>

#include "core_model.hpp"
#include "tactile_fwd.hpp"

class QPainter;

namespace tactile::gui {

class map_scene final : public QGraphicsScene
{
  Q_OBJECT

 public:
  explicit map_scene(not_null<model::tilemap*> map,
                     map_id id,
                     QWidget* parent = nullptr);

  void move_map_item(int dx, int dy);

  [[nodiscard]] auto id() const noexcept -> map_id
  {
    return m_id;
  }

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

 private:
  map_item* m_item;
  map_id m_id;
  QPointF m_lastMouseScenePos{};
};

}  // namespace tactile::gui
