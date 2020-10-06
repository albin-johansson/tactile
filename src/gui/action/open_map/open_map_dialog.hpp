#pragma once

#include <qfiledialog.h>

#include <concepts>  // invocable

namespace tactile::gui {

template <typename T>
concept OpenMapDialogCallback = std::invocable<T, const QString&>;

class open_map_dialog final : public QFileDialog
{
 public:
  explicit open_map_dialog(QWidget* parent = nullptr);

  template <OpenMapDialogCallback T>
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
