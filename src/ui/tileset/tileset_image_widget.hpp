#pragma once

#include <QWidget>
#include <memory>

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

  [[nodiscard]] static auto unique(const QImage& image,
                                   int tileWidth,
                                   int tileHeight,
                                   QWidget* parent = nullptr)
      -> std::unique_ptr<tileset_image_widget>;

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  std::unique_ptr<QLayout> m_layout;
  std::unique_ptr<QLabel> m_imageLabel;
  std::unique_ptr<QRubberBand> m_rubberBand;
  QPoint m_origin;
};

}  // namespace tactile::ui
