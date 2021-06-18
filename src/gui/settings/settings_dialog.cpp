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
#include "theme.hpp"
#include "theme_options_context_menu.hpp"
#include "to_integer.hpp"
#include "ui_settings_dialog.h"

namespace tactile {

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::SettingsDialog>(this)}
    , mThemeOptionsContextMenu{new ThemeOptionsContextMenu{this}}
    , mBasicPreview{new ColorPreviewManager{mUi->basicGroupLayout,
                                            QPalette::Active,
                                            mUi->themeTab}}
    , mDisabledPreview{new ColorPreviewManager{mUi->disabledGroupLayout,
                                               QPalette::Disabled,
                                               mUi->themeTab}}
{
  auto* validator = new QIntValidator{0, 9'999, this};
  mUi->tileWidthEdit->setValidator(validator);
  mUi->tileHeightEdit->setValidator(validator);

  mUi->themeComboBox->clear();

  for (const auto& name : GetStandardThemeNames())
  {
    mUi->themeComboBox->addItem(TactileLogo(), name);
  }

  mUi->themeComboBox->insertSeparator(mUi->themeComboBox->count());

  for (const auto& name : GetUserThemeNames())
  {
    mUi->themeComboBox->addItem(name);
  }

  FetchCurrentSettings();

  // clang-format off
  connect(this, &QDialog::accepted, this, &SettingsDialog::OnAccept);
  connect(mUi->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &SettingsDialog::OnApply);
  connect(mUi->themeOptionsButton, &QPushButton::pressed, this, &SettingsDialog::OnThemeOptionsButtonPressed);
  connect(mUi->generalRestoreDefaults, &QPushButton::clicked, this, &SettingsDialog::OnRestoreGeneralDefaults);
  connect(mUi->exportRestoreDefaults, &QPushButton::clicked, this, &SettingsDialog::OnRestoreExportDefaults);
  connect(mUi->addThemeButton, &QPushButton::pressed, this, &SettingsDialog::OnImportNewTheme);
  connect(mUi->themeComboBox, &QComboBox::currentTextChanged, this, &SettingsDialog::OnCurrentThemeChanged);

  connect(mThemeOptionsContextMenu, &ThemeOptionsContextMenu::S_RenameTheme, this, &SettingsDialog::OnRenameCurrentTheme);
  connect(mThemeOptionsContextMenu, &ThemeOptionsContextMenu::S_DuplicateTheme, this, &SettingsDialog::OnDuplicateCurrentTheme);
  connect(mThemeOptionsContextMenu, &ThemeOptionsContextMenu::S_ExportTheme, this, &SettingsDialog::OnExportCurrentTheme);
  connect(mThemeOptionsContextMenu, &ThemeOptionsContextMenu::S_ResetTheme, this, &SettingsDialog::OnResetCurrentTheme);
  connect(mThemeOptionsContextMenu, &ThemeOptionsContextMenu::S_RemoveTheme, this, &SettingsDialog::OnRemoveCurrentTheme);

  connect(mBasicPreview, &ColorPreviewManager::S_ColorChanged, this, &SettingsDialog::OnBasicColorChanged);
  connect(mDisabledPreview, &ColorPreviewManager::S_ColorChanged, this, &SettingsDialog::OnDisabledColorChanged);
  // clang-format on

  UpdateGeneralComponents();
  UpdateExportComponents();
  UpdateThemeComponents();
}

SettingsDialog::~SettingsDialog() noexcept = default;

void SettingsDialog::UpdateGeneralComponents()
{
  mUi->openglCheck->setChecked(mSnapshot.useOpenGL);
}

void SettingsDialog::UpdateExportComponents()
{
  mUi->embedTilesetsCheck->setChecked(mSnapshot.embedTilesets);
  mUi->readableOutputCheck->setChecked(mSnapshot.readableOutput);
  mUi->defaultFormatCombo->setCurrentText(mSnapshot.defaultFormat);
  mUi->generateDefaultsCheck->setChecked(mSnapshot.generateDefaults);
  mUi->tileWidthEdit->setText(QString::number(mSnapshot.tileWidth));
  mUi->tileHeightEdit->setText(QString::number(mSnapshot.tileHeight));
}

void SettingsDialog::UpdateThemeComponents()
{
  mUi->themeComboBox->setCurrentText(mSnapshot.theme);

  ColorPreviewButton::UpdateColor(*mUi->accentColorButton,
                                  prefs::AccentColor().Value());

  UpdateThemePreview();
}

void SettingsDialog::UpdateThemePreview()
{
  if (const auto palette = GetTheme(mUi->themeComboBox->currentText()))
  {
    mBasicPreview->UpdatePreview(*palette);
    mDisabledPreview->UpdatePreview(*palette);
  }
}

void SettingsDialog::FetchCurrentSettings()
{
  // General
  mSnapshot.useOpenGL = prefs::UseOpenGl().Value();

  // Export
  mSnapshot.embedTilesets = prefs::EmbedTilesets().Value();
  mSnapshot.readableOutput = prefs::UseReadableOutput().Value();
  mSnapshot.defaultFormat = prefs::DefaultFormat().Value();
  mSnapshot.generateDefaults = prefs::GenerateDefaults().Value();
  mSnapshot.tileWidth = prefs::TileWidth().Value();
  mSnapshot.tileHeight = prefs::TileHeight().Value();

  // Theme
  mSnapshot.theme = prefs::ThemeName().Value();

  QMapIterator iterator{prefs::UserThemes().Value()};
  while (iterator.hasNext())
  {
    iterator.next();

    const auto& name = iterator.key();
    if (!mUi->themeComboBox->findText(name))
    {
      mUi->themeComboBox->addItem(name);
    }
  }
}

void SettingsDialog::OnAccept()
{
  SetTheme(mUi->themeComboBox->currentText());
  emit S_ReloadTheme();

  if (const auto useOpenGL = mUi->openglCheck->isChecked();
      useOpenGL != mSnapshot.useOpenGL)
  {
    prefs::UseOpenGl() = useOpenGL;
    emit S_ReloadOpenGl(useOpenGL);
  }

  if (const auto defaultFormat = mUi->defaultFormatCombo->currentText();
      defaultFormat != mSnapshot.defaultFormat)
  {
    prefs::DefaultFormat() = defaultFormat;
  }

  if (const auto embedTilesets = mUi->embedTilesetsCheck->isChecked();
      embedTilesets != mSnapshot.embedTilesets)
  {
    prefs::EmbedTilesets() = embedTilesets;
  }

  if (const auto genDefaults = mUi->generateDefaultsCheck->isChecked();
      genDefaults != mSnapshot.generateDefaults)
  {
    prefs::GenerateDefaults() = genDefaults;
  }

  if (const auto readable = mUi->readableOutputCheck->isChecked();
      readable != mSnapshot.readableOutput)
  {
    prefs::UseReadableOutput() = readable;
  }

  if (const auto value = ToInteger(mUi->tileWidthEdit->text());
      value != mSnapshot.tileWidth)
  {
    prefs::TileWidth() = *value;
  }

  if (const auto value = ToInteger(mUi->tileHeightEdit->text());
      value != mSnapshot.tileHeight)
  {
    prefs::TileHeight() = *value;
  }
}

void SettingsDialog::OnApply()
{
  OnAccept();
  FetchCurrentSettings();
}

void SettingsDialog::OnRestoreGeneralDefaults()
{
  mUi->openglCheck->setChecked(prefs::use_opengl_def);
}

void SettingsDialog::OnRestoreExportDefaults()
{
  mUi->embedTilesetsCheck->setChecked(prefs::embed_tilesets_def);
  mUi->readableOutputCheck->setChecked(prefs::readable_output_def);
  mUi->defaultFormatCombo->setCurrentText(prefs::DefaultFormatDefault());
  mUi->generateDefaultsCheck->setChecked(prefs::generate_defaults_def);
  mUi->tileWidthEdit->setText(QString::number(prefs::tile_width_def));
  mUi->tileHeightEdit->setText(QString::number(prefs::tile_height_def));
}

void SettingsDialog::OnThemeOptionsButtonPressed()
{
  auto pos = mUi->themeOptionsButton->pos();

  const auto size = mUi->themeOptionsButton->size() / 2.0;
  pos += QPoint{size.width(), size.height()};

  mThemeOptionsContextMenu->exec(mapToGlobal(pos));
}

void SettingsDialog::OnRenameCurrentTheme()
{
  // TODO
}

void SettingsDialog::OnDuplicateCurrentTheme()
{
  const auto name = mUi->themeComboBox->currentText();
  if (const auto theme = GetTheme(name))
  {
    const auto newName = name + tr(" (Copy)");
    if (RegisterTheme(newName, *theme))
    {
      mUi->themeComboBox->addItem(newName);
    }
  }
}

void SettingsDialog::OnImportNewTheme()
{
  ImportThemeDialog::Spawn(
      [this](const QString& path) {
        const auto name = QFileInfo{path}.baseName();
        if (const auto palette = ParsePalette(path))
        {
          if (RegisterTheme(name, *palette))
          {
            mUi->themeComboBox->addItem(name);
          }
        }
        else
        {
          // TODO error message dialog
        }
      },
      this);
}

void SettingsDialog::OnExportCurrentTheme()
{
  ExportThemeDialog::Spawn(
      [this](const QString& path) {
        if (const auto theme = GetTheme(mUi->themeComboBox->currentText()))
        {
          SaveTheme(path, *theme);
        }
      },
      this);
}

void SettingsDialog::OnResetCurrentTheme()
{
  // TODO
}

void SettingsDialog::OnRemoveCurrentTheme()
{
  RemoveTheme(mUi->themeComboBox->currentText());
  mUi->themeComboBox->removeItem(mUi->themeComboBox->currentIndex());
}

void SettingsDialog::OnCurrentThemeChanged(const QString& name)
{
  UpdateThemePreview();

  const auto isStandardTheme = IsStandardTheme(name);
  mThemeOptionsContextMenu->SetResetEnabled(isStandardTheme);
  mThemeOptionsContextMenu->SetRenameEnabled(!isStandardTheme);
  mThemeOptionsContextMenu->SetRemoveEnabled(!isStandardTheme);
}

void SettingsDialog::OnThemeChanged(QPalette::ColorGroup group,
                                    QPalette::ColorRole role,
                                    const QColor& color)
{
  UpdateTheme(mUi->themeComboBox->currentText(), role, color, group);
}

void SettingsDialog::OnBasicColorChanged(const QPalette::ColorRole role,
                                         const QColor& color)
{
  OnThemeChanged(QPalette::Active, role, color);
}

void SettingsDialog::OnDisabledColorChanged(const QPalette::ColorRole role,
                                            const QColor& color)
{
  OnThemeChanged(QPalette::Disabled, role, color);
}

}  // namespace tactile
