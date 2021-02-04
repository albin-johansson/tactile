#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, tileset_empty_page)

namespace tactile::gui {

class tileset_empty_page final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_empty_page(QWidget* parent = nullptr);

  ~tileset_empty_page() noexcept override;

 signals:
  void ui_add_tileset();

 private:
  unique<Ui::tileset_empty_page> m_ui;
};

}  // namespace tactile::gui
