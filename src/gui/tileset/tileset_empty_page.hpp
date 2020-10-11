#pragma once

#include <qwidget.h>

#include "types.hpp"

namespace Ui {
class tileset_empty_page;
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
  Ui::tileset_empty_page* m_ui{};
};

}  // namespace tactile::gui
