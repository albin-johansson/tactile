#include "settings_dialog.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>
#include <optional>  // optional

#include "preferences.hpp"
#include "setting.hpp"
#include "theme.hpp"
#include "ui_settings_dialog.h"

namespace tactile::gui {
namespace {

[[nodiscard]] auto text_as_int(const QLineEdit* edit) -> std::optional<int>
{
  Q_ASSERT(edit);
  const auto text = edit->text();
  bool ok{};
  if (const auto value = text.toInt(&ok); ok) {
    return value;
  } else {
    return std::nullopt;
  }
}

}  // namespace

settings_dialog::settings_dialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::settings_dialog{}}
{
  m_ui->setupUi(this);

  auto* validator = new QIntValidator{0, 9'999, this};
  m_ui->tileWidthEdit->setValidator(validator);
  m_ui->tileHeightEdit->setValidator(validator);

  fetch_current_settings();

  // clang-format off
  connect(this, &QDialog::accepted, this, &settings_dialog::handle_accept);
  connect(m_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &settings_dialog::handle_apply);
  connect(m_ui->exportRestoreDefaults, &QPushButton::clicked, this, &settings_dialog::handle_export_restore_defaults);
  // clang-format on

  update_general_components();
  update_export_components();
  update_appearance_components();
}

settings_dialog::~settings_dialog() noexcept
{
  delete m_ui;
}

void settings_dialog::update_general_components()
{}

void settings_dialog::update_export_components()
{
  m_ui->embedTilesetsCheck->setChecked(m_embedTilesets);
  m_ui->readableOutputCheck->setChecked(m_readableOutput);
  m_ui->defaultFormatCombo->setCurrentText(m_defaultFormat);
  m_ui->generateDefaultsCheck->setChecked(m_generateDefaults);
  m_ui->tileWidthEdit->setText(QString::number(m_tileWidth));
  m_ui->tileHeightEdit->setText(QString::number(m_tileHeight));
}

void settings_dialog::update_appearance_components()
{
  m_ui->themeComboBox->setCurrentText(m_theme);
}

void settings_dialog::handle_accept()
{
  if (const auto theme = m_ui->themeComboBox->currentText(); theme != m_theme) {
    theme::set_theme(theme);
    emit reload_theme();
  }

  if (const auto defaultFormat = m_ui->defaultFormatCombo->currentText();
      defaultFormat != m_defaultFormat) {
    prefs::saves::default_format().set(defaultFormat);
  }

  if (const auto embedTilesets = m_ui->embedTilesetsCheck->isChecked();
      embedTilesets != m_embedTilesets) {
    prefs::saves::embed_tilesets().set(embedTilesets);
  }

  if (const auto genDefaults = m_ui->generateDefaultsCheck->isChecked();
      genDefaults != m_generateDefaults) {
    prefs::saves::generate_defaults().set(genDefaults);
  }

  if (const auto readable = m_ui->readableOutputCheck->isChecked();
      readable != m_readableOutput) {
    prefs::saves::readable_output().set(readable);
  }

  if (const auto value = text_as_int(m_ui->tileWidthEdit); value) {
    prefs::saves::tile_width().set(*value);
  }

  if (const auto value = text_as_int(m_ui->tileHeightEdit); value) {
    prefs::saves::tile_height().set(*value);
  }
}

void settings_dialog::handle_apply()
{
  handle_accept();
  fetch_current_settings();
}

void settings_dialog::handle_export_restore_defaults()
{
  using namespace prefs;
  m_ui->embedTilesetsCheck->setChecked(saves::embed_tilesets_def());
  m_ui->readableOutputCheck->setChecked(saves::readable_output_def());
  m_ui->defaultFormatCombo->setCurrentText(saves::default_format_def());
  m_ui->generateDefaultsCheck->setChecked(saves::generate_defaults_def());
  m_ui->tileWidthEdit->setText(QString::number(saves::tile_width_def()));
  m_ui->tileHeightEdit->setText(QString::number(saves::tile_height_def()));
}

void settings_dialog::fetch_current_settings()
{
  m_theme = prefs::graphics::theme_name().value();
  m_defaultFormat = prefs::saves::default_format().value();
  m_tileWidth = prefs::saves::tile_width().value();
  m_tileHeight = prefs::saves::tile_height().value();
  m_embedTilesets = prefs::saves::embed_tilesets().value();
  m_generateDefaults = prefs::saves::generate_defaults().value();
  m_readableOutput = prefs::saves::readable_output().value();
}

}  // namespace tactile::gui
