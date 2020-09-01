#pragma once

#include <QGraphicsView>
#include <QRect>

#include "core_model.hpp"
#include "map_scene.hpp"

namespace tactile::gui {

/**
 * @class map_view
 *
 * @brief Represents the UI component that presents a map.
 *
 * @since 0.1.0
 *
 * @headerfile map_view.hpp
 */
class map_view final : public QGraphicsView
{
  Q_OBJECT

 public:
  explicit map_view(not_null<model::tilemap*> map,
                    map_id id,
                    QWidget* parent = nullptr);

  void center_map();

  void move_map(int dx, int dy);

  [[nodiscard]] auto id() const -> map_id;

  void force_redraw();  // instead of this, add set_width/height that updates
                        // the underlying map item

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  //  bool event(QEvent* event) override;

 private:
  QPoint m_lastMousePos{};
  QPointF m_lastMouseScenePos{};

  [[nodiscard]] auto get_map_scene() -> map_scene*;

  [[nodiscard]] auto get_map_scene() const -> const map_scene*;
};

}  // namespace tactile::gui
