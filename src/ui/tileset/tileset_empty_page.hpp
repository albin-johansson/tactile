#pragma once

#include <QWidget>

#include "tactile_types.hpp"

namespace Ui {
class TilesetEmptyPageUI;
}

namespace tactile::ui {

class tileset_empty_page final : public QWidget {
  Q_OBJECT

 public:
  explicit tileset_empty_page(QWidget* parent = nullptr);

  ~tileset_empty_page() noexcept override;

 signals:
  void s_requested_tileset();

 private:
  owner<Ui::TilesetEmptyPageUI*> m_ui;
};

}  // namespace tactile::ui
