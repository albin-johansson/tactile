#pragma once

#include <QFileDialog>  // QFileDialog
#include <concepts>     // invocable

namespace tactile {

class SelectFileDialog final : public QFileDialog
{
 public:
  explicit SelectFileDialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void Spawn(T&& callable)
  {
    SelectFileDialog dialog;
    if (dialog.exec())
    {
      Q_ASSERT(dialog.selectedFiles().size() == 1);
      callable(dialog.selectedFiles().front());
    }
  }
};

}  // namespace tactile
