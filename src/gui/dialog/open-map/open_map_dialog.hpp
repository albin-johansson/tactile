#pragma once

#include <QFileDialog>  // QFileDialog
#include <concepts>     // invocable

namespace tactile {

class OpenMapDialog final : public QFileDialog
{
 public:
  explicit OpenMapDialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void Spawn(T&& callback)
  {
    OpenMapDialog dialog;
    if (dialog.exec())
    {
      for (const auto& file : dialog.selectedFiles())
      {
        callback(file);
      }
    }
  }
};

}  // namespace tactile
