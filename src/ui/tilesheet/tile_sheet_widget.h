#pragma once
#include <QWidget>

#include "smart_pointers.h"

namespace Ui {
class TileSheetWidgetUI;
}

namespace tactile {

class TileSheetContentPage;
class TileSheetEmptyPage;

class TileSheetWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit TileSheetWidget(QWidget* parent = nullptr);

  ~TileSheetWidget() noexcept override;

  void add_tile_sheet(int id, const Shared<QImage>& image) noexcept;

  void remove_tile_sheet(int id) noexcept;

 signals:
  void s_requested_tile_sheet();

 private:
  Ui::TileSheetWidgetUI* m_ui;
  TileSheetContentPage* m_contentPage;
  TileSheetEmptyPage* m_emptyPage;
  int m_emptyIndex;
  int m_contentIndex;
};

}  // namespace tactile
