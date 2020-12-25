#pragma once

#include <QFileDialog>
#include <concepts>  // invocable

namespace tactile::gui {

class select_file_dialog final : public QFileDialog
{
 public:
  explicit select_file_dialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void spawn(T&& callable)
  {
    select_file_dialog dialog;
    if (dialog.exec()) {
      Q_ASSERT(dialog.selectedFiles().size() == 1);
      callable(dialog.selectedFiles().front());
    }
  }
};

}  // namespace tactile::gui
