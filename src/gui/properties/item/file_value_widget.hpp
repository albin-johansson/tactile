#pragma once

#include <QFileInfo>

#include "property_value_widget.hpp"

namespace Ui {
class file_value_widget;
}

namespace tactile::gui {

class file_value_widget final : public property_value_widget
{
  Q_OBJECT

 public:
  explicit file_value_widget(QWidget* parent = nullptr);

  void reset_path();

  void set_path(const QFileInfo& path);

 signals:
  void spawn_dialog();

 private:
  Ui::file_value_widget* m_ui{};
};

}  // namespace tactile::gui
