#pragma once

#include <QFileDialog>
#include <concepts>  // invocable

namespace tactile::gui {

class open_map_dialog final : public QFileDialog
{
 public:
  explicit open_map_dialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void spawn(T&& callback)
  {
    open_map_dialog dialog;
    if (dialog.exec()) {
      for (const auto& file : dialog.selectedFiles()) {
        callback(file);
      }
    }
  }
};

}  // namespace tactile::gui
