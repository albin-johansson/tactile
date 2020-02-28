#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QWidget>

namespace Ui {
class MainWindow;
}

namespace tactile {

/**
 * The <code>TactileWindow</code> class is a subclass of
 * <code>QOpenGLWindow</code> that represents the window used in the Tactile
 * application.
 *
 * @see QOpenGLWindow
 * @since 0.1.0
 */
class TactileWindow final : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @since 0.1.0
   */
  explicit TactileWindow(QWidget* parent = nullptr);

  ~TactileWindow() override;

 public slots:
  void render() noexcept;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  Ui::MainWindow* m_ui;
  QOpenGLWidget* m_renderSurface;

  /**
   * Returns the bounds of the window.
   *
   * @return the bounds of the window.
   * @since 0.1.0
   */
  [[nodiscard]] QRect get_render_surface_bounds() const noexcept;
};

}  // namespace tactile
