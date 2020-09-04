#pragma once

#include <qlabel.h>

#include <memory>

#include "types.hpp"

namespace tactile::gui {

class tileset_image_label final : public QLabel
{
 public:
  explicit tileset_image_label(const QImage& image,
                               tile_width tileWidth,
                               tile_height tileHeight,
                               QWidget* parent = nullptr);

  ~tileset_image_label() noexcept override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  tile_width m_tileWidth;
  tile_height m_tileHeight;
  int m_width;
  int m_height;
  int m_maxX;
  int m_maxY;
  int m_nRows;
  int m_nCols;
};

}  // namespace tactile::gui
