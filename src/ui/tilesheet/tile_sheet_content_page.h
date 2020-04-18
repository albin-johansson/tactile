#pragma once

#include <QWidget>

#include "smart_pointers.h"
#include "tactile_types.h"

namespace Ui {
class TileSheetContentPageUI;
}

namespace tactile {

class TileSheetTab;

class TileSheetContentPage final : public QWidget {
  Q_OBJECT

 public:
  explicit TileSheetContentPage(QWidget* parent = nullptr);

  ~TileSheetContentPage() noexcept override;

  void add_tile_sheet(int id, const Shared<QImage>& image) noexcept;

  void remove_tile_sheet(int id) noexcept;

  [[nodiscard]] bool empty() const noexcept;

 signals:
  void s_requested_tile_sheet();

 private:
  Ui::TileSheetContentPageUI* m_ui;
  HashMap<int, Shared<TileSheetTab>> m_tabs;

  //  [[nodiscard]] bool contains_tile_sheet(int id) const noexcept;
};

}  // namespace tactile
