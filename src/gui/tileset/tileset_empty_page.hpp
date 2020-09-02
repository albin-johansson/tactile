#pragma once

#include <qwidget.h>

#include "tactile_types.hpp"

namespace Ui {
class TilesetEmptyPageUI; // FIXME
}

namespace tactile::gui {

class tileset_empty_page final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_empty_page(QWidget* parent = nullptr);

  ~tileset_empty_page() noexcept override;

 signals:
  void request_new_tileset();

 private:
  owner<Ui::TilesetEmptyPageUI*> m_ui;
};

}  // namespace tactile::gui
