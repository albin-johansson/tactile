#pragma once

#include <QWidget>
#include <unordered_map>

#include "smart_pointers.h"

namespace Ui {
class TileSheetContentPageUI;
}

namespace tactile {

class TileSheetTab;

class TileSheetContentPage final : public QWidget {
 public:
  explicit TileSheetContentPage(QWidget* parent = nullptr);

  ~TileSheetContentPage() noexcept override;

  void add_tile_sheet(int id, Shared<QImage> image) noexcept;

  void remove_tile_sheet(int id) noexcept;

  [[nodiscard]] bool empty() const noexcept;

 private:
  Ui::TileSheetContentPageUI* m_ui;
  std::unordered_map<int, Unique<TileSheetTab>> m_tabs;

//  [[nodiscard]] bool contains_tile_sheet(int id) const noexcept;
};

}  // namespace tactile
