#pragma once
#include <QWidget>

namespace tactile {

class TileSheetTab final : public QWidget {
 public:
  explicit TileSheetTab(QWidget* parent = nullptr);

  ~TileSheetTab() noexcept override;

 private:
};

}  // namespace tactile
