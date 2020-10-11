#pragma once

#include <qevent.h>
#include <qwidget.h>

#include <optional>

#include "map_document.hpp"
#include "map_tab_widget.hpp"
#include "types.hpp"

namespace Ui {
class map_editor;
}

class QTabWidget;

namespace tactile::gui {

/**
 * @class map_editor
 *
 * @brief Represents the center stage of the editor and contains the main
 * map editor pane.
 *
 * @since 0.1.0
 *
 * @headerfile map_editor.hpp
 */
class map_editor final : public QWidget
{
  Q_OBJECT

 public:
  explicit map_editor(QWidget* parent = nullptr);

  ~map_editor() noexcept override;

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  void add_new_map_tab(core::map_document* map,
                       map_id id,
                       const QString& title);

  void select_tab(map_id id);

  void close_tab(map_id id) noexcept;

  void center_viewport();

  void move_map(int dx, int dy) noexcept;

  /**
   * @brief Enables the startup view.
   *
   * @details The startup view is the main page show upon startup or when no
   * maps are active.
   *
   * @since 0.1.0
   */
  void enable_startup_view() noexcept;

  /**
   * @brief Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enable_editor_view() noexcept;

  [[nodiscard]] auto in_editor_mode() const -> bool;

  [[nodiscard]] auto active_tab_id() const -> std::optional<map_id>;

  [[nodiscard]] auto active_tab_name() const -> std::optional<QString>;

  [[nodiscard]] auto num_tabs() const -> int;

 public slots:
  void handle_redraw();

 signals:
  void request_remove_tab(map_id id);

  void request_select_tab(map_id id);

  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);

  void mouse_moved(QMouseEvent* event, QPointF mapPosition);

  void mouse_released(QMouseEvent* event, QPointF mapPosition);

  void mouse_entered(QEvent* event);

  void mouse_exited(QEvent* event);

  void theme_changed();

 private:
  Ui::map_editor* m_ui{};
  map_tab_widget* m_mapTabWidget{};
  int m_editorID{};
  int m_startupID{};

  void init_connections();

 private slots:
  void tab_changed(int index);
};

}  // namespace tactile::gui
