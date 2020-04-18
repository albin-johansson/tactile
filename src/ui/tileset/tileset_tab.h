#pragma once
#include <QWidget>

#include "smart_pointers.h"

class QImage;
class QLayout;

namespace tactile {

class TilesetScrollArea;
class TilesetImageWidget;

class TilesetTab final : public QWidget {
 public:
  explicit TilesetTab(const QImage& image, QWidget* parent = nullptr);

  ~TilesetTab() noexcept override;

 private:
  Unique<TilesetScrollArea> m_scrollArea;
  Unique<TilesetImageWidget> m_imageWidget;
  Unique<QLayout> m_layout;
};

}  // namespace tactile
