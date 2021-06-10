#pragma once

#include <QLabel>  // QLabel

#include "tileset.hpp"

namespace tactile {

class TilesetImageLabel final : public QLabel
{
 public:
  explicit TilesetImageLabel(const core::Tileset& tileset,
                             QWidget* parent = nullptr);

  ~TilesetImageLabel() noexcept override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  tile_width mTileWidth;
  tile_height mTileHeight;
  int mWidth{};
  int mHeight{};
  int mMaxX{};
  int mMaxY{};
};

}  // namespace tactile
