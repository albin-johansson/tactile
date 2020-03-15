#pragma once
#include <QWidget>

namespace Ui {

class CentralWidgetUI;

}

namespace tactile {

class RenderPane;

/**
 * The <code>CentralWidget</code> class represents the center stage of the
 * editor and contains the main tile map editor pane.
 *
 * @since 0.1.0
 */
class CentralWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit CentralWidget(QWidget* parent = nullptr);

  ~CentralWidget() noexcept override;

  void center_viewport(int mapWidth, int mapHeight) noexcept;

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

  [[nodiscard]] const QRect& current_viewport() const noexcept;

  [[nodiscard]] bool in_editor_mode() const noexcept;

//  [[nodiscard, deprecated]] QWidget* render_widget() noexcept;

 public slots:
  void trigger_redraw() noexcept;

 signals:
  void req_redraw(QPainter& painter);

 private:
  Ui::CentralWidgetUI* m_ui;
  RenderPane* m_renderPane;
  int m_editorViewIndex;
  int m_startupViewIndex;
};

}  // namespace tactile
