#pragma once

#include <QFileDialog>  // QFileDialog
#include <concepts>     // invocable

namespace tactile {

class ImportThemeDialog final : public QFileDialog
{
 public:
  explicit ImportThemeDialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void Spawn(T&& callable, QWidget* parent = nullptr)
  {
    ImportThemeDialog dialog{parent};
    if (dialog.exec())
    {
      Q_ASSERT(dialog.selectedFiles().size() == 1);
      callable(dialog.selectedFiles().front());
    }
  }
};

}  // namespace tactile
