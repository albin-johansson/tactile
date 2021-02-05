#include "settings_dialog.hpp"

#include <QComboBox>    // QComboBox
#include <QPushButton>  // QPushButton

#include "color_preview_button.hpp"
#include "export_theme_dialog.hpp"
#include "import_theme_dialog.hpp"
#include "init_ui.hpp"
#include "maybe.hpp"
#include "parse_palette.hpp"
#include "preferences.hpp"
#include "string_utils.hpp"
#include "theme.hpp"
#include "theme_options_context_menu.hpp"
#include "ui_settings_dialog.h"

namespace tactile::gui {

using namespace prefs;

settings_dialog::settings_dialog(QWidget* parent)
    : QDialog{parent}
    , m_ui{init_ui<Ui::settings_dialog>(this)}
    , m_themeOptionsContextMenu{new theme_options_context_menu{this}}
    , m_basicPreview{m_ui->basicGroupLayout}
{
  auto* validator = new QIntValidator{0, 9'999, this};
  m_ui->tileWidthEdit->setValidator(validator);
  m_ui->tileHeightEdit->setValidator(validator);

  m_ui->themeComboBox->clear();
  for (const auto& name : get_all_theme_names()) {
    m_ui->themeComboBox->addItem(name);
  }

  fetch_current_settings();

  // clang-format off
  connect(this, &QDialog::accepted,
          this, &settings_dialog::handle_accept);

  connect(m_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
          this, &settings_dialog::apply);

  connect(m_ui->themeOptionsButton, &QPushButton::pressed,
          this, &settings_dialog::pressed_theme_options_button);

  connect(m_ui->exportRestoreDefaults, &QPushButton::clicked,
          this, &settings_dialog::restore_export_defaults);

  connect(m_ui->appearanceRestoreDefaults, &QPushButton::clicked,
          this, &settings_dialog::restore_appearance_defaults);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::duplicate_theme,
          this, &settings_dialog::duplicate_current_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::import_theme,
          this, &settings_dialog::import_new_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::export_theme,
          this, &settings_dialog::export_current_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::reset_theme,
          this, &settings_dialog::reset_current_theme);

  connect(m_ui->themeComboBox, &QComboBox::currentTextChanged,
          [this](const QString& str) {
            update_theme_preview();
          });
  // clang-format on

  update_general_components();
  update_export_components();
  update_theme_components();
}

settings_dialog::~settings_dialog() noexcept = default;

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

void settings_dialog::update_theme_components()
{
  m_ui->themeComboBox->setCurrentText(m_theme);
  update_theme_preview();
}

void settings_dialog::update_theme_preview()
{
  if (const auto palette = get_theme(m_ui->themeComboBox->currentText())) {
    m_basicPreview.update_preview(*palette);
  }
}

void settings_dialog::handle_accept()
{
  if (const auto theme = m_ui->themeComboBox->currentText(); theme != m_theme) {
    if (set_theme(theme)) {
      // Only emit signal if we successfully changed the current theme
      emit reload_theme();
    }
  }

  if (const auto defaultFormat = m_ui->defaultFormatCombo->currentText();
      defaultFormat != m_defaultFormat) {
    saves::default_format().set(defaultFormat);
  }

  if (const auto embedTilesets = m_ui->embedTilesetsCheck->isChecked();
      embedTilesets != m_embedTilesets) {
    saves::embed_tilesets().set(embedTilesets);
  }

  if (const auto genDefaults = m_ui->generateDefaultsCheck->isChecked();
      genDefaults != m_generateDefaults) {
    saves::generate_defaults().set(genDefaults);
  }

  if (const auto readable = m_ui->readableOutputCheck->isChecked();
      readable != m_readableOutput) {
    saves::readable_output().set(readable);
  }

  if (const auto value = to_integer(m_ui->tileWidthEdit->text());
      value != m_tileWidth) {
    saves::tile_width().set(*value);
  }

  if (const auto value = to_integer(m_ui->tileHeightEdit->text());
      value != m_tileHeight) {
    saves::tile_height().set(*value);
  }
}

void settings_dialog::apply()
{
  handle_accept();
  fetch_current_settings();
}

void settings_dialog::restore_export_defaults()
{
  m_ui->embedTilesetsCheck->setChecked(saves::embed_tilesets_def());
  m_ui->readableOutputCheck->setChecked(saves::readable_output_def());
  m_ui->defaultFormatCombo->setCurrentText(saves::default_format_def());
  m_ui->generateDefaultsCheck->setChecked(saves::generate_defaults_def());
  m_ui->tileWidthEdit->setText(QString::number(saves::tile_width_def()));
  m_ui->tileHeightEdit->setText(QString::number(saves::tile_height_def()));
}

void settings_dialog::restore_appearance_defaults()
{
  //  m_ui->themeComboBox->setCurrentText(gfx::theme_name_def());
}

void settings_dialog::fetch_current_settings()
{
  // General
  // TODO...

  // Export
  m_embedTilesets = saves::embed_tilesets().value();
  m_readableOutput = saves::readable_output().value();
  m_defaultFormat = saves::default_format().value();
  m_generateDefaults = saves::generate_defaults().value();
  m_tileWidth = saves::tile_width().value();
  m_tileHeight = saves::tile_height().value();

  // Theme
  m_theme = gfx::theme_name().value();
  for (const auto& name : gfx::user_themes().value().keys()) {
    if (!m_ui->themeComboBox->findText(name)) {
      m_ui->themeComboBox->addItem(name);
    }
  }
}

void settings_dialog::pressed_theme_options_button()
{
  m_themeOptionsContextMenu->exec(mapToGlobal(m_ui->themeOptionsButton->pos()));
}

void settings_dialog::duplicate_current_theme()
{
  // TODO
}

void settings_dialog::import_new_theme()
{
  import_theme_dialog::spawn(
      [this](const QString& path) {
        const auto name = QFileInfo{path}.baseName();
        if (const auto palette = parse_palette(path)) {
          if (register_theme(name, *palette)) {
            m_ui->themeComboBox->addItem(name);
          }
        } else {
          // TODO error message dialog
        }
      },
      this);
}

void settings_dialog::export_current_theme()
{
  export_theme_dialog::spawn(
      [this](const QString& path) {
        // TODO
      },
      this);
}

void settings_dialog::reset_current_theme()
{
  // TODO
}

}  // namespace tactile::gui
