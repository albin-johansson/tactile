#pragma once

#include <QWidget>
#include <optional>

namespace Ui {
class CentralWidgetUI;
}

class QTabWidget;

namespace tactile::ui {

class TileMapTabWidget;

/**
 * The <code>CentralEditorWidget</code> class represents the center stage of the
 * editor and contains the main tilemap editor pane.
 *
 * @since 0.1.0
 */
class CentralEditorWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit CentralEditorWidget(QWidget* parent = nullptr);

  ~CentralEditorWidget() noexcept override;

  int add_new_map_tab(const QString& title) noexcept;

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

  [[nodiscard]] bool in_editor_mode() const noexcept;

  [[nodiscard]] std::optional<int> active_tab_id() const noexcept;

  [[nodiscard]] int open_tabs() const noexcept;

 public slots:
  void trigger_redraw() noexcept;

 signals:
  void s_redraw(QPainter& painter);

  void s_removed_tab(int id);

  void s_selected_tab(int id);

 private:
  Ui::CentralWidgetUI* m_ui;
  TileMapTabWidget* m_mapTabWidget;
  int m_editorViewIndex;
  int m_startupViewIndex;

  void init_connections() noexcept;
};

}  // namespace tactile::ui
