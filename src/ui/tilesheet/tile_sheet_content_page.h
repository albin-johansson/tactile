#pragma once

#include <QWidget>

namespace Ui {
class TileSheetContentPageUI;
}

namespace tactile {

class TileSheetContentPage final : public QWidget {
 public:
  explicit TileSheetContentPage(QWidget* parent = nullptr);

  ~TileSheetContentPage() noexcept override;

 private:
  Ui::TileSheetContentPageUI* m_ui;
};

}  // namespace tactile
