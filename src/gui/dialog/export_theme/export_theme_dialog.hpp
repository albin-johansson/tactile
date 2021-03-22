#pragma once

#include <QFileDialog>  // QFileDialog
#include <concepts>     // invocable

namespace tactile::gui {

class export_theme_dialog final : public QFileDialog
{
 public:
  explicit export_theme_dialog(QWidget* parent = nullptr);

  template <std::invocable<const QString&> T>
  static void spawn(T&& callable, QWidget* parent = nullptr)
  {
    export_theme_dialog dialog{parent};
    if (dialog.exec())
    {
      Q_ASSERT(dialog.selectedFiles().size() == 1);
      callable(dialog.selectedFiles().front());
    }
  }
};

}  // namespace tactile::gui
