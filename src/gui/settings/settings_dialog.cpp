#include "settings_dialog.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>

#include "init_ui.hpp"
#include "maybe.hpp"
#include "preferences.hpp"
#include "setting.hpp"
#include "tactile_qstring.hpp"
#include "theme.hpp"
#include "ui_settings_dialog.h"

namespace tactile::gui {
namespace {

[[nodiscard]] auto text_as_int(const QLineEdit* edit) -> maybe<int>
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

using namespace prefs;

settings_dialog::settings_dialog(QWidget* parent)
    : QDialog{parent}
    , m_ui{init_ui<Ui::settings_dialog>(this)}
{
  auto* validator = new QIntValidator{0, 9'999, this};
  m_ui->tileWidthEdit->setValidator(validator);
  m_ui->tileHeightEdit->setValidator(validator);

  fetch_current_settings();

  // clang-format off
  connect(this, &QDialog::accepted,
          this, &settings_dialog::handle_accept);

  connect(m_ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
          this, &settings_dialog::apply);

  connect(m_ui->exportRestoreDefaults, &QPushButton::clicked,
          this, &settings_dialog::restore_export_defaults);

  connect(m_ui->appearanceRestoreDefaults, &QPushButton::clicked,
          this, &settings_dialog::restore_appearance_defaults);

  connect(m_ui->themeComboBox, &QComboBox::currentTextChanged,
          [this](const QString& str) {
            update_theme_preview();
          });
  // clang-format on

  update_general_components();
  update_export_components();
  update_theme_components();
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

void settings_dialog::update_theme_components()
{
  m_ui->themeComboBox->setCurrentText(m_theme);
  update_theme_preview();
}

void settings_dialog::update_theme_preview()
{
  if (const auto theme = theme::from_name(m_ui->themeComboBox->currentText())) {
    static const auto fmt = TACTILE_QSTRING(u"background-color: %1");

    const auto update =
        [](QPushButton* button, QLineEdit* edit, const QBrush& brush) {
          const auto name = brush.color().name(QColor::HexArgb);
          edit->setText(name);
          button->setStyleSheet(fmt.arg(name));
        };

    // clang-format off
    update(m_ui->basicBasePreview, m_ui->basicBaseEdit, theme->base());
    update(m_ui->basicAlternateBasePreview, m_ui->basicAlternateBaseEdit, theme->alternateBase());
    update(m_ui->basicWindowPreview, m_ui->basicWindowEdit, theme->window());
    update(m_ui->basicWindowTextPreview, m_ui->basicWindowTextEdit, theme->windowText());
    update(m_ui->basicLightPreview, m_ui->basicLightEdit, theme->light());
    update(m_ui->basicMidLightPreview, m_ui->basicMidLightEdit, theme->midlight());
    update(m_ui->basicMidPreview, m_ui->basicMidEdit, theme->mid());
    update(m_ui->basicDarkPreview, m_ui->basicDarkEdit, theme->dark());
    update(m_ui->basicLinkPreview, m_ui->basicLinkEdit, theme->link());
    update(m_ui->basicLinkVisitedPreview, m_ui->basicLinkVisitedEdit, theme->linkVisited());
    update(m_ui->basicButtonPreview, m_ui->basicButtonEdit, theme->button());
    update(m_ui->basicButtonTextPreview, m_ui->basicButtonTextEdit, theme->buttonText());
    update(m_ui->basicHighlightPreview, m_ui->basicHighlightEdit, theme->highlight());
    update(m_ui->basicHighlightedTextPreview, m_ui->basicHighlightedTextEdit, theme->highlightedText());
    update(m_ui->basicPlaceholderTextPreview, m_ui->basicPlaceholderTextEdit, theme->placeholderText());
    update(m_ui->basicTooltipBasePreview, m_ui->basicTooltipBaseEdit, theme->toolTipBase());
    update(m_ui->basicTooltipTextPreview, m_ui->basicTooltipTextEdit, theme->toolTipText());
    update(m_ui->basicBrightTextPreview, m_ui->basicBrightTextEdit, theme->brightText());
    update(m_ui->basicTextPreview, m_ui->basicTextEdit, theme->text());
    update(m_ui->basicShadowPreview, m_ui->basicShadowEdit, theme->shadow());
    // clang-format on
  }
}

void settings_dialog::handle_accept()
{
  if (const auto theme = m_ui->themeComboBox->currentText(); theme != m_theme) {
    theme::set_theme(theme);
    emit reload_theme();
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

  if (const auto value = text_as_int(m_ui->tileWidthEdit);
      value != m_tileWidth) {
    saves::tile_width().set(*value);
  }

  if (const auto value = text_as_int(m_ui->tileHeightEdit);
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
  m_ui->themeComboBox->setCurrentText(gfx::theme_name_def());
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

  // Appearance
  m_theme = gfx::theme_name().value();
}

}  // namespace tactile::gui
