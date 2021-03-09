#pragma once

#include <QLabel>  // QLabel

#include "tileset.hpp"

namespace tactile::gui {

class tileset_image_label final : public QLabel
{
 public:
  explicit tileset_image_label(const core::tileset& tileset,
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
};

}  // namespace tactile::gui
