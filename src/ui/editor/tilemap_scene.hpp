#pragma once

#include <QGraphicsScene>
#include <QRect>
#include <QWidget>

class QPainter;

namespace tactile::ui {

class tilemap_scene final : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit tilemap_scene(int id, QWidget* parent = nullptr);

//  void center_viewport(int mapWidth, int mapHeight) noexcept;

//  void move_viewport(int dx, int dy) noexcept;

  //  void set_viewport_size(const QSize& size) noexcept;

  [[nodiscard]] auto id() const noexcept -> int;

 signals:
  void request_redraw(QPainter& painter);

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

  void drawForeground(QPainter* painter, const QRectF& rect) override;

  //  void paintEvent(QPaintEvent* event) override;

  //  void resizeEvent(QResizeEvent* event) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

 private:
//  QRect m_viewport{};
  int m_id;
  QPointF m_lastMouseScenePos{};
};

}  // namespace tactile::ui
