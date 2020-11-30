#pragma once

#include <QWidget>

namespace Ui {
class properties_widget;
}

namespace tactile::gui {

class properties_widget final : public QWidget
{
 public:
  explicit properties_widget(QWidget* parent = nullptr);

 private:
  Ui::properties_widget* m_ui{};
};

}  // namespace tactile::gui
