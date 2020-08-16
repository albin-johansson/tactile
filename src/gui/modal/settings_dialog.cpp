#include "settings_dialog.hpp"

#include "ui_settings.h"

namespace tactile::gui {

settings_dialog::settings_dialog(QWidget* parent)
    : QDialog{parent}, m_ui{new Ui::SettingsUI{}}
{
  m_ui->setupUi(this);
}

settings_dialog::~settings_dialog() noexcept
{
  delete m_ui;
}

}  // namespace tactile::gui
