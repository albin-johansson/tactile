#pragma once
#include <QLabel>

#include "smart_pointers.hpp"

namespace tactile {

class TilesetImageLabel final : public QLabel {
 public:
  explicit TilesetImageLabel(const QImage& image,
                             int tileWidth,
                             int tileHeight,
                             QWidget* parent = nullptr);

  ~TilesetImageLabel() noexcept override;

  [[nodiscard]] static Unique<TilesetImageLabel> unique(
      const QImage& image,
      int tileWidth,
      int tileHeight,
      QWidget* parent = nullptr);

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

}  // namespace tactile
