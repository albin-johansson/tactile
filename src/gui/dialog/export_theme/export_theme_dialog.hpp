#pragma once

#include <QFileDialog>  // QFileDialog
#include <concepts>     // invocable

namespace tactile::gui {

class ExportThemeDialog final : public QFileDialog
{
 public:
  explicit ExportThemeDialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void Spawn(T&& callable, QWidget* parent = nullptr)
  {
    ExportThemeDialog dialog{parent};
    if (dialog.exec())
    {
      Q_ASSERT(dialog.selectedFiles().size() == 1);
      callable(dialog.selectedFiles().front());
    }
  }
};

}  // namespace tactile::gui
