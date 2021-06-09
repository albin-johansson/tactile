#include "about_dialog.hpp"

#include "init_ui.hpp"
#include "ui_about_dialog.h"

namespace tactile {

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog{parent}
    , mUi{init_ui<Ui::AboutDialog>(this)}
{}

AboutDialog::~AboutDialog() noexcept = default;

void AboutDialog::Spawn()
{
  AboutDialog dialog;
  dialog.exec();
}

}  // namespace tactile
