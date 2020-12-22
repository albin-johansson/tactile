#pragma once

#include <QWidget>

namespace tactile::gui {

class property_value_widget : public QWidget
{
  Q_OBJECT

 public:
  explicit property_value_widget(QWidget* parent = nullptr) : QWidget{parent}
  {}

  virtual void enter_active_mode()
  {}

  virtual void enter_idle_mode()
  {}
};

}  // namespace tactile::gui
