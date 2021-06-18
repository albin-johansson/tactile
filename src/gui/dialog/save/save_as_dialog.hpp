#pragma once

#include <QFileDialog>  // QFileDialog
#include <concepts>     // invocable

namespace tactile {

class SaveAsDialog final : public QFileDialog
{
 public:
  explicit SaveAsDialog(const QString& fileName, QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void Spawn(T&& callback, const QString& fileName)
  {
    SaveAsDialog dialog{fileName};
    if (dialog.exec())
    {
      callback(dialog.selectedFiles().first());
    }
  }
};

}  // namespace tactile
