#pragma once

#include <QGraphicsScene>
#include <QRect>
#include <QWidget>

#include "core_model.hpp"

class QPainter;

namespace tactile::gui {

class tilemap_scene final : public QGraphicsScene
{
  Q_OBJECT

 public:
  explicit tilemap_scene(not_null<model::tilemap*> map,
                         int id,
                         QWidget* parent = nullptr);

  //  void center_viewport(int mapWidth, int mapHeight) noexcept;

  //  void move_viewport(int dx, int dy) noexcept;

  //  void set_viewport_size(const QSize& size) noexcept;

  [[nodiscard]] auto id() const noexcept -> int;

 signals:
  [[deprecated]] void request_redraw(QPainter& painter, const QRectF& exposed);

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

 private:
  //  QRect m_viewport{};
  int m_id;
  QPointF m_lastMouseScenePos{};
};

}  // namespace tactile::gui
