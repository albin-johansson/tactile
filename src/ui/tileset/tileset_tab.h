#pragma once
#include <QWidget>

#include "smart_pointers.h"

class QImage;
class QLayout;
class QScrollArea;

namespace tactile {

class TilesetImageWidget;

class TilesetTab final : public QWidget {
 public:
  explicit TilesetTab(const QImage& image,
                      int tileWidth,
                      int tileHeight,
                      QWidget* parent = nullptr);

  ~TilesetTab() noexcept override;

 private:
  Unique<QScrollArea> m_scrollArea;
  Unique<TilesetImageWidget> m_imageWidget;
  Unique<QLayout> m_layout;
};

}  // namespace tactile
