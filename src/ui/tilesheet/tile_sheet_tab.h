#pragma once
#include <QWidget>

#include "smart_pointers.h"

class QImage;
class QLayout;

namespace tactile {

class TileSheetScrollArea;
class TileSheetImageWidget;

class TileSheetTab final : public QWidget {
 public:
  explicit TileSheetTab(const QImage& image, QWidget* parent = nullptr);

  ~TileSheetTab() noexcept override;

 private:
  Unique<TileSheetScrollArea> m_scrollArea;
  Unique<TileSheetImageWidget> m_imageWidget;
  Unique<QLayout> m_layout;
};

}  // namespace tactile
