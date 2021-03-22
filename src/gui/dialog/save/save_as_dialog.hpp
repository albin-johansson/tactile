#pragma once

#include <QFileDialog>  // QFileDialog
#include <QString>      // QString
#include <concepts>     // invocable

namespace tactile::gui {

class save_as_dialog final : public QFileDialog
{
 public:
  explicit save_as_dialog(const QString& fileName, QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void spawn(T&& callback, const QString& fileName)
  {
    save_as_dialog dialog{fileName};
    if (dialog.exec())
    {
      callback(dialog.selectedFiles().first());
    }
  }
};

}  // namespace tactile::gui
