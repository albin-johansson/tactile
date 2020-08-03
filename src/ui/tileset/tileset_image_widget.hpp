#pragma once

#include <QWidget>

class QLayout;
class QLabel;
class QImage;
class QRubberBand;

namespace tactile::ui {

class tileset_image_widget final : public QWidget {
  Q_OBJECT

 public:
  explicit tileset_image_widget(const QImage& image,
                                int tileWidth,
                                int tileHeight,
                                QWidget* parent = nullptr);

  ~tileset_image_widget() noexcept override;

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QLayout* m_layout;
  QLabel* m_imageLabel;
  QRubberBand* m_rubberBand;
  QPoint m_origin;
};

}  // namespace tactile::ui
