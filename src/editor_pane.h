#pragma once

#include <QOpenGLWidget>
#include <QRect>

namespace tactile {

class EditorPane final : public QOpenGLWidget {
 public:
  explicit EditorPane(QWidget* parent = nullptr) noexcept;

 protected:
  void initializeGL() override;

  void resizeGL(int w, int h) override;

  void paintGL() override;

 private:
  [[nodiscard]] QRect get_bounds() const noexcept;
};

}  // namespace tactile