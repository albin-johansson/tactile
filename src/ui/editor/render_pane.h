#pragma once
#include <QRect>
#include <QWidget>

class QPainter;

namespace tactile {

class RenderPane final : public QWidget { // TODO rename to TilemapWidget...
  Q_OBJECT

 public:
  explicit RenderPane(QWidget* parent = nullptr);

  void center_viewport(int mapWidth, int mapHeight);

  [[nodiscard]] const QRect& viewport() const noexcept
  {
    return m_viewport;
  }

 signals:
  void rp_req_redraw(QPainter& painter);

 protected:
  void paintEvent(QPaintEvent* event) override;

  void resizeEvent(QResizeEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QRect m_viewport;
  int m_lastMouseX;
  int m_lastMouseY;
};

}  // namespace tactile
