#pragma once
#include <QWidget>

#include "smart_pointers.h"

class QLabel;
class QScrollArea;
class QImage;
class QLayout;

namespace tactile {

class TileSheetTab final : public QWidget {
 public:
  explicit TileSheetTab(const QImage& image, QWidget* parent = nullptr);

  ~TileSheetTab() noexcept override;

 private:
  Unique<QScrollArea> m_scrollArea;
  Unique<QLabel> m_imageLabel;
  Unique<QLayout> m_layout;
};

}  // namespace tactile
