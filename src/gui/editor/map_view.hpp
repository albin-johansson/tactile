#pragma once

#include <QGraphicsView>  // QGraphicsView
#include <QMouseEvent>    // QMouseEvent
#include <QRect>          // QRect

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::gui, map_scene)

namespace tactile::gui {

/**
 * \class map_view
 *
 * \brief Represents the UI component that presents a map.
 *
 * \details This class also handles the mouse input for maps, and delegates
 * mouse events to the model that will be used by the mouse tools.
 *
 * \since 0.1.0
 *
 * \headerfile map_view.hpp
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

  void show_properties();

  [[nodiscard]] auto id() const -> map_id;

 public slots:
  void set_opengl_enabled(bool enabled);

 signals:
  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);
  void mouse_moved(QMouseEvent* event, QPointF mapPosition);
  void mouse_released(QMouseEvent* event, QPointF mapPosition);
  void mouse_entered(QEvent* event);
  void mouse_exited(QEvent* event);
  void increase_zoom();
  void decrease_zoom();
  void spawn_context_menu(const QPoint& pos);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void enterEvent(QEnterEvent* event) override;

  void leaveEvent(QEvent* event) override;

  void wheelEvent(QWheelEvent* event) override;

 private:
  QPoint m_lastMousePos{};

  [[nodiscard]] auto get_map_scene() -> map_scene*;

  [[nodiscard]] auto get_map_scene() const -> const map_scene*;
};

}  // namespace tactile::gui
