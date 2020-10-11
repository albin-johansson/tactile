#pragma once

#include <qgraphicsview.h>
#include <qrect.h>

#include "map_document.hpp"
#include "map_scene.hpp"
#include "model.hpp"

namespace tactile::gui {

/**
 * @class map_view
 *
 * @brief Represents the UI component that presents a map.
 *
 * @details This class also handles the mouse input for maps, and delegates
 * mouse events to the model that will be used by the mouse tools.
 *
 * @since 0.1.0
 *
 * @headerfile map_view.hpp
 */
class map_view final : public QGraphicsView
{
  Q_OBJECT

 public:
  explicit map_view(core::map_document* map,
                    map_id id,
                    QWidget* parent = nullptr);

  void force_redraw();

  void center_map();

  void move_map(int dx, int dy);

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  [[nodiscard]] auto id() const -> map_id;

 signals:
  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);

  void mouse_moved(QMouseEvent* event, QPointF mapPosition);

  void mouse_released(QMouseEvent* event, QPointF mapPosition);

  void mouse_entered(QEvent* event);

  void mouse_exited(QEvent* event);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void enterEvent(QEvent* event) override;

  void leaveEvent(QEvent* event) override;

 private:
  QPoint m_lastMousePos{};

  [[nodiscard]] auto get_map_scene() -> map_scene*;

  [[nodiscard]] auto get_map_scene() const -> const map_scene*;
};

}  // namespace tactile::gui
