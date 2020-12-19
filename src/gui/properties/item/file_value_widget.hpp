#pragma once

#include <QFileInfo>
#include <QWidget>

namespace Ui {
class file_value_widget;
}

namespace tactile::gui {

class file_value_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit file_value_widget(QWidget* parent = nullptr);

  void set_path(const QFileInfo& path);

  void set_visible(bool visible);

  [[deprecated("See set_visible")]] void setVisible(bool visible) override
  {
    // This function does nothing by design as a workaround
  }

 signals:
  void spawn_dialog();

 private:
  Ui::file_value_widget* m_ui{};
};

}  // namespace tactile::gui
