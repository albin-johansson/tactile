#pragma once

#include <QWidget>

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
  void ui_add_tileset();

 private:
  Ui::tileset_empty_page* m_ui{};
};

}  // namespace tactile::gui
