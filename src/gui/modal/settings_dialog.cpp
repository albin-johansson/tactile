#include "settings_dialog.hpp"

#include "setting.hpp"
#include "setting_identifiers.hpp"
#include "theme.hpp"
#include "ui_settings.h"

namespace tactile::gui {

settings_dialog::settings_dialog(QWidget* parent)
    : QDialog{parent}, m_ui{new Ui::SettingsUI{}}
{
  m_ui->setupUi(this);

  connect(this, &QDialog::accepted, this, &settings_dialog::handle_accept);

  prefs::setting<QString> themeName{prefs::id::graphics::theme_name()};
  Q_ASSERT(themeName);

  m_ui->themeComboBox->setCurrentText(*themeName);
}

settings_dialog::~settings_dialog() noexcept
{
  delete m_ui;
}

void settings_dialog::handle_accept()
{
  // TODO don't touch settings that didn't change
  prefs::theme::set_theme(m_ui->themeComboBox->currentText());
}

}  // namespace tactile::gui
