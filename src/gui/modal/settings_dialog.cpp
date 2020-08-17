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

  m_initialTheme = *themeName;
  m_ui->themeComboBox->setCurrentText(m_initialTheme);
}

settings_dialog::~settings_dialog() noexcept
{
  delete m_ui;
}

void settings_dialog::handle_accept()
{
  if (const auto theme = m_ui->themeComboBox->currentText();
      theme != m_initialTheme) {
    prefs::theme::set_theme(theme);
    emit reload_theme();
  }
}

}  // namespace tactile::gui
