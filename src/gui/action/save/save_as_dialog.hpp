#pragma once

#include <qfiledialog.h>
#include <qstring.h>
#include <qurl.h>

#include <concepts>  // invocable

namespace tactile::gui {

template <typename T>
concept SaveAsDialogCallback = std::invocable<T, const QUrl&>;

class save_as_dialog final : public QFileDialog
{
 public:
  explicit save_as_dialog(const QString& fileName, QWidget* parent = nullptr);

  template <SaveAsDialogCallback T>
  static void spawn(T&& callback, const QString& fileName)
  {
    save_as_dialog dialog{fileName};
    if (dialog.exec()) {
      callback(dialog.selectedUrls().at(0));
    }
  }
};

}  // namespace tactile::gui
