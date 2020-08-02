#include "settings_dialog.hpp"

#include "ui_settings.h"

namespace tactile {

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog{parent}, m_ui{new Ui::SettingsUI{}}
{
  m_ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() noexcept
{
  delete m_ui;
}

}  // namespace tactile
