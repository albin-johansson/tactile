#include "about_dialog.h"

#include "ui_about.h"

namespace tactile {

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog{parent}, m_ui{new Ui::AboutUI{}}
{
  m_ui->setupUi(this);
}

AboutDialog::~AboutDialog() noexcept
{
  delete m_ui;
}

}  // namespace tactile
