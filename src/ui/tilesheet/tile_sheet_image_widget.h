#pragma once
#include <QWidget>

#include "smart_pointers.h"

class QLayout;
class QLabel;
class QImage;
class QRubberBand;

namespace tactile {

class TileSheetImageWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit TileSheetImageWidget(const QImage& image, QWidget* parent = nullptr);

  ~TileSheetImageWidget() noexcept override;

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
