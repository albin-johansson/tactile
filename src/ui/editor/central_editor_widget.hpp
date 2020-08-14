#pragma once

#include <QWidget>
#include <optional>

#include "tactile_fwd.hpp"

namespace Ui {
class CentralWidgetUI;
}

class QTabWidget;

namespace tactile::ui {

/**
 * @class central_editor_widget
 *
 * @brief Represents the center stage of the editor and contains the main
 * tilemap editor pane.
 *
 * @since 0.1.0
 *
 * @headerfile central_editor_widget.hpp
 */
class central_editor_widget final : public QWidget {
  Q_OBJECT

 public:
  explicit central_editor_widget(QWidget* parent = nullptr);

  ~central_editor_widget() noexcept override;

  auto add_new_map_tab(model::core* core, const QString& title) noexcept -> int;

  [[nodiscard]] auto next_tab_id() const noexcept -> int;

  void close_tab(int id) noexcept;

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  /**
   * Enables the startup view.
   *
   * @since 0.1.0
   */
  void enable_startup_view() noexcept;

  /**
   * Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enable_editor_view() noexcept;

  [[nodiscard]] auto in_editor_mode() const noexcept -> bool;

  [[nodiscard]] auto active_tab_id() const noexcept -> std::optional<int>;

  [[nodiscard]] auto open_tabs() const noexcept -> int;

 public slots:
  void handle_trigger_redraw() noexcept;

 signals:
  void request_redraw(QPainter& painter, const QRectF& exposed);

  void request_remove_tab(int id);

  void request_select_tab(int id);

 private:
  Ui::CentralWidgetUI* m_ui;
  tilemap_tab* m_mapTabWidget;
  int m_editorViewIndex;
  int m_startupViewIndex;

  void init_connections() noexcept;
};

}  // namespace tactile::ui
