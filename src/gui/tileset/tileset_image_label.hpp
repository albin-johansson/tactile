#pragma once

#include <QLabel>
#include <memory>

namespace tactile::gui {

class tileset_image_label final : public QLabel {
 public:
  explicit tileset_image_label(const QImage& image,
                               int tileWidth,
                               int tileHeight,
                               QWidget* parent = nullptr);

  ~tileset_image_label() noexcept override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  int m_tileWidth;
  int m_tileHeight;
  int m_width;
  int m_height;
  int m_maxX;
  int m_maxY;
  int m_nRows;
  int m_nCols;
};

}  // namespace tactile::gui
