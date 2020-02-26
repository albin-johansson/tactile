#pragma once
#include <QOpenGLWindow>

namespace tactile {

/**
 * The <code>TactileWindow</code> class is a subclass of
 * <code>QOpenGLWindow</code> that represents the window used in the Tactile
 * application.
 *
 * @see QOpenGLWindow
 * @since 0.1.0
 */
class TactileWindow final : public QOpenGLWindow {
 public:
  /**
   * @since 0.1.0
   */
  TactileWindow();

 protected:
  void initializeGL() override;

  void resizeGL(int w, int h) override;

  void paintGL() override;

 private:
  /**
   * Returns the bounds of the window.
   *
   * @return the bounds of the window.
   * @since 0.1.0
   */
  [[nodiscard]] QRect get_bounds() const noexcept;
};

}  // namespace tactile
