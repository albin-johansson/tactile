#pragma once

#include <QGraphicsView>
#include <QRect>

#include "core_model.hpp"
#include "tilemap_scene.hpp"

namespace tactile::ui {

/**
 * @class tilemap_view
 *
 * @brief Represents the UI component that holds a tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile tilemap_view.hpp
 */
class tilemap_view final : public QGraphicsView {
  Q_OBJECT

 public:
  explicit tilemap_view(model::core_model* core, int id, QWidget* parent = nullptr);

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto id() const noexcept -> int;

 signals:
  void request_redraw(QPainter& painter, const QRectF& exposed);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  //  void paintEvent(QPaintEvent* event) override;
  //
  void resizeEvent(QResizeEvent* event) override;

 private:
  QPoint m_lastMousePos{};
  QPointF m_lastMouseScenePos{};

  [[nodiscard]] auto get_tilemap_scene() noexcept -> tilemap_scene*;
  [[nodiscard]] auto get_scene() const noexcept -> const tilemap_scene*;
};

}  // namespace tactile::gui
