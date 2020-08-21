#pragma once

#include <QWidget>
#include <optional>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

namespace Ui {
class main_editor;
}

class QTabWidget;

namespace tactile::gui {

/**
 * @class main_editor
 *
 * @brief Represents the center stage of the editor and contains the main
 * tilemap editor pane.
 *
 * @since 0.1.0
 *
 * @headerfile main_editor.hpp
 */
class main_editor final : public QWidget
{
  Q_OBJECT

 public:
  explicit main_editor(QWidget* parent = nullptr);

  ~main_editor() noexcept override;

  void add_new_map_tab(not_null<model::tilemap*> map,
                       const QString& title,
                       int id) noexcept;

  void select_tab(int id);

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
  void handle_redraw();

 signals:
  void request_remove_tab(int id);

  void request_select_tab(int id);

  void theme_changed();

 private:
  Ui::main_editor* m_ui{};
  tilemap_tab* m_mapTabWidget{};
  int m_editorID{};
  int m_startupID{};

  void init_connections() noexcept;

 private slots:
  void tab_changed(int index);
};

}  // namespace tactile::gui
