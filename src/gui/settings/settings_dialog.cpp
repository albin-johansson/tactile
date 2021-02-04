#include "settings_dialog.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QValidator>

#include "color_preview_button.hpp"
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
    , m_basicBasePreview{new color_preview_button{Qt::black, this}}
    , m_basicAlternateBasePreview{new color_preview_button{Qt::black, this}}
    , m_basicWindowPreview{new color_preview_button{Qt::black, this}}
    , m_basicWindowTextPreview{new color_preview_button{Qt::black, this}}
    , m_basicLightPreview{new color_preview_button{Qt::black, this}}
    , m_basicMidLightPreview{new color_preview_button{Qt::black, this}}
    , m_basicDarkPreview{new color_preview_button{Qt::black, this}}
    , m_basicLinkPreview{new color_preview_button{Qt::black, this}}
    , m_basicLinkVisitedPreview{new color_preview_button{Qt::black, this}}
    , m_basicButtonPreview{new color_preview_button{Qt::black, this}}
    , m_basicButtonTextPreview{new color_preview_button{Qt::black, this}}
    , m_basicHighlightPreview{new color_preview_button{Qt::black, this}}
    , m_basicHighlightedTextPreview{new color_preview_button{Qt::black, this}}
    , m_basicPlaceholderTextPreview{new color_preview_button{Qt::black, this}}
    , m_basicTooltipBasePreview{new color_preview_button{Qt::black, this}}
    , m_basicTooltipTextPreview{new color_preview_button{Qt::black, this}}
    , m_basicTextPreview{new color_preview_button{Qt::black, this}}
    , m_basicShadowPreview{new color_preview_button{Qt::black, this}}
{
  auto* validator = new QIntValidator{0, 9'999, this};
  m_ui->tileWidthEdit->setValidator(validator);
  m_ui->tileHeightEdit->setValidator(validator);

  // clang-format off
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Base"), m_basicBasePreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Alternate Base"), m_basicAlternateBasePreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Window"), m_basicWindowPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Window Text"), m_basicWindowTextPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Light"), m_basicLightPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Mid Light"), m_basicMidLightPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Dark"), m_basicDarkPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Link"), m_basicLinkPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Link (visited)"), m_basicLinkVisitedPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Button"), m_basicButtonPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Button Text"), m_basicButtonTextPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Highlight"), m_basicHighlightPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Highlighted Text"), m_basicHighlightedTextPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Placeholder Text"), m_basicPlaceholderTextPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Tooltip Base"), m_basicTooltipBasePreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Tooltip Text"), m_basicTooltipTextPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Text"), m_basicTextPreview);
  m_ui->basicGroupLayout->addRow(TACTILE_QSTRING(u"Shadow"), m_basicShadowPreview);
  // clang-format on

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
  if (const auto theme = theme::from_name(m_ui->themeComboBox->currentText())) {
    m_basicBasePreview->set_color(theme->base().color());
    m_basicAlternateBasePreview->set_color(theme->alternateBase().color());
    m_basicWindowPreview->set_color(theme->window().color());
    m_basicWindowTextPreview->set_color(theme->windowText().color());
    m_basicLightPreview->set_color(theme->light().color());
    m_basicMidLightPreview->set_color(theme->midlight().color());
    m_basicDarkPreview->set_color(theme->dark().color());
    m_basicLinkPreview->set_color(theme->link().color());
    m_basicLinkVisitedPreview->set_color(theme->linkVisited().color());
    m_basicButtonPreview->set_color(theme->button().color());
    m_basicButtonTextPreview->set_color(theme->buttonText().color());
    m_basicHighlightPreview->set_color(theme->highlight().color());
    m_basicHighlightedTextPreview->set_color(theme->highlightedText().color());
    m_basicPlaceholderTextPreview->set_color(theme->placeholderText().color());
    m_basicTooltipBasePreview->set_color(theme->toolTipBase().color());
    m_basicTooltipTextPreview->set_color(theme->toolTipText().color());
    m_basicTextPreview->set_color(theme->text().color());
    m_basicShadowPreview->set_color(theme->shadow().color());
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
