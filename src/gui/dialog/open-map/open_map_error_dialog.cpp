#include "open_map_error_dialog.hpp"

#include "init_ui.hpp"
#include "ui_open_map_error_dialog.h"

namespace tactile {

OpenMapErrorDialog::OpenMapErrorDialog(QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::OpenMapErrorDialog>(this)}
{}

OpenMapErrorDialog::~OpenMapErrorDialog() noexcept = default;

void OpenMapErrorDialog::SetFile(const QFileInfo& path)
{
  mUi->filePathLabel->setText(path.fileName());
}

void OpenMapErrorDialog::SetErrorMessage(const QString& message)
{
  mUi->errorMessageLabel->setText(message);
}

}  // namespace tactile
