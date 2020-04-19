#pragma once
#include <QWidget>

#include "smart_pointers.h"

class QLayout;
class QLabel;
class QImage;
class QRubberBand;

namespace tactile {

class TilesetImageWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetImageWidget(const QImage& image,
                              int tileWidth,
                              int tileHeight,
                              QWidget* parent = nullptr);

  ~TilesetImageWidget() noexcept override;

  [[nodiscard]] static Unique<TilesetImageWidget> unique(
      const QImage& image,
      int tileWidth,
      int tileHeight,
      QWidget* parent = nullptr);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  Unique<QLayout> m_layout;
  Unique<QLabel> m_imageLabel;
  Unique<QRubberBand> m_rubberBand;
  QPoint m_origin;
};

}  // namespace tactile
