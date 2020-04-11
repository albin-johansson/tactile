#pragma once
#include <QWidget>

#include "smart_pointers.h"

namespace Ui {
class TileSheetWidgetUI;
}

namespace tactile {

class TileSheetContentPage;

class TileSheetWidget final : public QWidget {
 public:
  explicit TileSheetWidget(QWidget* parent = nullptr);

  ~TileSheetWidget() noexcept override;

  void add_tile_sheet(int id, Shared<QImage> image) noexcept;

  void remove_tile_sheet(int id) noexcept;

 private:
  Ui::TileSheetWidgetUI* m_ui;
  TileSheetContentPage* m_contentPage;
  int m_emptyIndex;
  int m_contentIndex;
};

}  // namespace tactile
