#pragma once
#include <QWidget>

namespace Ui {
class TileSheetEmptyPageUI;
}

namespace tactile {

class TileSheetEmptyPage final : public QWidget {
 public:
  explicit TileSheetEmptyPage(QWidget* parent = nullptr);

  ~TileSheetEmptyPage() noexcept override;

 private:
  Ui::TileSheetEmptyPageUI* m_ui;
};

}  // namespace tactile
