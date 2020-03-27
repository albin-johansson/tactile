#pragma once
#include <QWidget>

namespace Ui {
class TileSheetWidgetUI;
}

namespace tactile {

class TileSheetWidget final : public QWidget {
 public:
  explicit TileSheetWidget(QWidget* parent = nullptr);

  ~TileSheetWidget() noexcept override;

 private:
  Ui::TileSheetWidgetUI* m_ui;
};

}  // namespace tactile
