#pragma once
#include <QWidget>

namespace Ui {
class TileSheetEmptyPageUI;
}

namespace tactile {

class TileSheetEmptyPage final : public QWidget {
  Q_OBJECT

 public:
  explicit TileSheetEmptyPage(QWidget* parent = nullptr);

  ~TileSheetEmptyPage() noexcept override;

 signals:
  void s_requested_tile_sheet();

 private:
  Ui::TileSheetEmptyPageUI* m_ui;
};

}  // namespace tactile
