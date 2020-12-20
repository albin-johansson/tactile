#pragma once

#include <QWidget>

namespace tactile::gui {

class property_value_widget : public QWidget
{
 public:
  explicit property_value_widget(QWidget* parent = nullptr) : QWidget{parent}
  {}

  void set_visible(const bool visible)
  {
    QWidget::setVisible(visible);
  }

  [[deprecated("See set_visible")]] void setVisible(bool visible) final
  {
    // This function does nothing by design as a workaround
  }
};

}  // namespace tactile::gui
