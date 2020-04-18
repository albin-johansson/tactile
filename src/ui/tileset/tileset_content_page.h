#pragma once

#include <QWidget>

#include "smart_pointers.h"
#include "tactile_types.h"

namespace Ui {
class TilesetContentPageUI;
}

namespace tactile {

class TilesetTab;

class TilesetContentPage final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetContentPage(QWidget* parent = nullptr);

  ~TilesetContentPage() noexcept override;

  void add_tileset(int id, const Shared<QImage>& image) noexcept;

  void remove_tileset(int id) noexcept;

  [[nodiscard]] bool empty() const noexcept;

 signals:
  void s_requested_tileset();

 private:
  Ui::TilesetContentPageUI* m_ui;
  HashMap<int, Shared<TilesetTab>> m_tabs;

  //  [[nodiscard]] bool contains_tileset(int id) const noexcept;
};

}  // namespace tactile
