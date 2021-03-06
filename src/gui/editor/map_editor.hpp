#pragma once

#include <QMouseEvent>  // QMouseEvent
#include <QWidget>      // QWidget

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"

namespace Ui {
class map_editor;
}

TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::gui, map_tab_widget)
TACTILE_FORWARD_DECLARE(tactile::gui, map_editor_context_menu)

namespace tactile::gui {

/**
 * \class map_editor
 *
 * \brief Represents the center stage of the editor and contains the main
 * map editor pane.
 *
 * \since 0.1.0
 *
 * \headerfile map_editor.hpp
 */
class map_editor final : public QWidget
{
  Q_OBJECT

 public:
  explicit map_editor(QWidget* parent = nullptr);

  ~map_editor() noexcept override;

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  void add_map_tab(core::map_document* map, map_id id, const QString& title);

  void select_tab(map_id id);

  void close_tab(map_id id) noexcept;

  void center_viewport();

  void move_map(int dx, int dy) noexcept;

  /**
   * \brief Enables the startup view.
   *
   * \details The startup view is the main page show upon startup or when no
   * maps are active.
   *
   * \since 0.1.0
   */
  void enable_startup_view() noexcept;

  /**
   * \brief Enables the main editor view.
   *
   * \since 0.1.0
   */
  void enable_editor_view() noexcept;

  /**
   * \copydoc map_tab_widget::set_active_tab_name()
   */
  void set_active_tab_name(const QString& name);

  [[nodiscard]] auto in_editor_mode() const -> bool;

  [[nodiscard]] auto active_tab_id() const -> maybe<map_id>;

  [[nodiscard]] auto active_tab_name() const -> maybe<QString>;

  [[nodiscard]] auto tab_count() const -> int;

 public slots:
  void force_redraw();

 signals:
  void ui_remove_map(map_id id);
  void ui_select_map(map_id id);
  void increase_zoom();
  void decrease_zoom();
  void theme_changed();
  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);
  void mouse_moved(QMouseEvent* event, QPointF mapPosition);
  void mouse_released(QMouseEvent* event, QPointF mapPosition);
  void mouse_entered(QEvent* event);
  void mouse_exited(QEvent* event);

 private:
  unique<Ui::map_editor> m_ui;
  map_tab_widget* m_tabWidget{};
  map_editor_context_menu* m_contextMenu{};
  int m_editorID{};
  int m_startupID{};

  void init_connections();

 private slots:
  void tab_changed(int index);

  void spawn_context_menu(const QPoint& pos);

  void show_map_properties();
};

}  // namespace tactile::gui
