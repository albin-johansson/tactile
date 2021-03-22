#include "settings_dialog.hpp"

#include <QComboBox>    // QComboBox
#include <QPushButton>  // QPushButton

#include "color_preview_button.hpp"
#include "export_theme_dialog.hpp"
#include "icons.hpp"
#include "import_theme_dialog.hpp"
#include "init_ui.hpp"
#include "maybe.hpp"
#include "parse_palette.hpp"
#include "preferences.hpp"
#include "save_theme.hpp"
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
    , m_basicPreview{new color_preview_manager{m_ui->basicGroupLayout,
                                               QPalette::Active,
                                               m_ui->themeTab}}
    , m_disabledPreview{new color_preview_manager{m_ui->disabledGroupLayout,
                                                  QPalette::Disabled,
                                                  m_ui->themeTab}}
{
  auto* validator = new QIntValidator{0, 9'999, this};
  m_ui->tileWidthEdit->setValidator(validator);
  m_ui->tileHeightEdit->setValidator(validator);

  m_ui->themeComboBox->clear();

  for (const auto& name : get_standard_theme_names())
  {
    m_ui->themeComboBox->addItem(icons::tactile_logo(), name);
  }

  m_ui->themeComboBox->insertSeparator(m_ui->themeComboBox->count());

  for (const auto& name : get_user_theme_names())
  {
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

  connect(m_ui->generalRestoreDefaults, &QPushButton::clicked,
          this, &settings_dialog::restore_general_defaults);

  connect(m_ui->exportRestoreDefaults, &QPushButton::clicked,
          this, &settings_dialog::restore_export_defaults);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::rename_theme,
          this, &settings_dialog::rename_current_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::duplicate_theme,
          this, &settings_dialog::duplicate_current_theme);

  connect(m_ui->addThemeButton, &QPushButton::pressed,
          this, &settings_dialog::import_new_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::export_theme,
          this, &settings_dialog::export_current_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::reset_theme,
          this, &settings_dialog::reset_current_theme);

  connect(m_themeOptionsContextMenu, &theme_options_context_menu::remove_theme,
          this, &settings_dialog::remove_current_theme);

  connect(m_basicPreview, &color_preview_manager::color_changed,
          [this](const QPalette::ColorRole role, const QColor& color) {
            theme_changed(QPalette::Active, role, color);
          });

  connect(m_disabledPreview, &color_preview_manager::color_changed,
          [this](const QPalette::ColorRole role, const QColor& color) {
            theme_changed(QPalette::Disabled, role, color);
          });

  connect(m_ui->themeComboBox, &QComboBox::currentTextChanged,
          this, &settings_dialog::when_current_theme_changed);
  // clang-format on

  update_general_components();
  update_export_components();
  update_theme_components();
}

settings_dialog::~settings_dialog() noexcept = default;

void settings_dialog::update_general_components()
{
  m_ui->openglCheck->setChecked(m_snapshot.useOpenGL);
}

void settings_dialog::update_export_components()
{
  m_ui->embedTilesetsCheck->setChecked(m_snapshot.embedTilesets);
  m_ui->readableOutputCheck->setChecked(m_snapshot.readableOutput);
  m_ui->defaultFormatCombo->setCurrentText(m_snapshot.defaultFormat);
  m_ui->generateDefaultsCheck->setChecked(m_snapshot.generateDefaults);
  m_ui->tileWidthEdit->setText(QString::number(m_snapshot.tileWidth));
  m_ui->tileHeightEdit->setText(QString::number(m_snapshot.tileHeight));
}

void settings_dialog::update_theme_components()
{
  m_ui->themeComboBox->setCurrentText(m_snapshot.theme);

  color_preview_button::update_color(*m_ui->accentColorButton,
                                     prefs::gfx::accent_color().value());

  update_theme_preview();
}

void settings_dialog::update_theme_preview()
{
  if (const auto palette = get_theme(m_ui->themeComboBox->currentText()))
  {
    m_basicPreview->update_preview(*palette);
    m_disabledPreview->update_preview(*palette);
  }
}

void settings_dialog::handle_accept()
{
  set_theme(m_ui->themeComboBox->currentText());
  emit reload_theme();

  if (const auto useOpenGL = m_ui->openglCheck->isChecked();
      useOpenGL != m_snapshot.useOpenGL)
  {
    gfx::use_opengl().set(useOpenGL);
    emit reload_opengl(useOpenGL);
  }

  if (const auto defaultFormat = m_ui->defaultFormatCombo->currentText();
      defaultFormat != m_snapshot.defaultFormat)
  {
    saves::default_format().set(defaultFormat);
  }

  if (const auto embedTilesets = m_ui->embedTilesetsCheck->isChecked();
      embedTilesets != m_snapshot.embedTilesets)
  {
    saves::embed_tilesets().set(embedTilesets);
  }

  if (const auto genDefaults = m_ui->generateDefaultsCheck->isChecked();
      genDefaults != m_snapshot.generateDefaults)
  {
    saves::generate_defaults().set(genDefaults);
  }

  if (const auto readable = m_ui->readableOutputCheck->isChecked();
      readable != m_snapshot.readableOutput)
  {
    saves::readable_output().set(readable);
  }

  if (const auto value = to_integer(m_ui->tileWidthEdit->text());
      value != m_snapshot.tileWidth)
  {
    saves::tile_width().set(*value);
  }

  if (const auto value = to_integer(m_ui->tileHeightEdit->text());
      value != m_snapshot.tileHeight)
  {
    saves::tile_height().set(*value);
  }
}

void settings_dialog::apply()
{
  handle_accept();
  fetch_current_settings();
}

void settings_dialog::restore_general_defaults()
{
  m_ui->openglCheck->setChecked(gfx::use_opengl_def());
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

void settings_dialog::fetch_current_settings()
{
  // General
  m_snapshot.useOpenGL = gfx::use_opengl().value();

  // Export
  m_snapshot.embedTilesets = saves::embed_tilesets().value();
  m_snapshot.readableOutput = saves::readable_output().value();
  m_snapshot.defaultFormat = saves::default_format().value();
  m_snapshot.generateDefaults = saves::generate_defaults().value();
  m_snapshot.tileWidth = saves::tile_width().value();
  m_snapshot.tileHeight = saves::tile_height().value();

  // Theme
  m_snapshot.theme = gfx::theme_name().value();
  for (const auto& name : gfx::user_themes().value().keys())
  {
    if (!m_ui->themeComboBox->findText(name))
    {
      m_ui->themeComboBox->addItem(name);
    }
  }
}

void settings_dialog::pressed_theme_options_button()
{
  auto pos = m_ui->themeOptionsButton->pos();

  const auto size = m_ui->themeOptionsButton->size() / 2.0;
  pos += QPoint{size.width(), size.height()};

  m_themeOptionsContextMenu->exec(mapToGlobal(pos));
}

void settings_dialog::rename_current_theme()
{
  // TODO
}

void settings_dialog::duplicate_current_theme()
{
  const auto name = m_ui->themeComboBox->currentText();
  if (const auto theme = get_theme(name))
  {
    const auto newName = name + tr(" (Copy)");
    if (register_theme(newName, *theme))
    {
      m_ui->themeComboBox->addItem(newName);
    }
  }
}

void settings_dialog::import_new_theme()
{
  import_theme_dialog::spawn(
      [this](const QString& path) {
        const auto name = QFileInfo{path}.baseName();
        if (const auto palette = parse_palette(path))
        {
          if (register_theme(name, *palette))
          {
            m_ui->themeComboBox->addItem(name);
          }
        }
        else
        {
          // TODO error message dialog
        }
      },
      this);
}

void settings_dialog::export_current_theme()
{
  export_theme_dialog::spawn(
      [this](const QString& path) {
        if (const auto theme = get_theme(m_ui->themeComboBox->currentText()))
        {
          save_theme(path, *theme);
        }
      },
      this);
}

void settings_dialog::reset_current_theme()
{
  // TODO
}

void settings_dialog::remove_current_theme()
{
  remove_theme(m_ui->themeComboBox->currentText());
  m_ui->themeComboBox->removeItem(m_ui->themeComboBox->currentIndex());
}

void settings_dialog::when_current_theme_changed(const QString& name)
{
  update_theme_preview();

  const auto isStandardTheme = is_standard_theme(name);
  m_themeOptionsContextMenu->set_reset_enabled(isStandardTheme);
  m_themeOptionsContextMenu->set_rename_enabled(!isStandardTheme);
  m_themeOptionsContextMenu->set_remove_enabled(!isStandardTheme);
}

void settings_dialog::theme_changed(const QPalette::ColorGroup group,
                                    const QPalette::ColorRole role,
                                    const QColor& color)
{
  //  qDebug("theme_changed");
  update_theme(m_ui->themeComboBox->currentText(), role, color, group);
  //  emit reload_theme();
}

}  // namespace tactile::gui
