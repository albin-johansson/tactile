#pragma once

#include <QDialog>  // QDialog
#include <QString>  // QString

#include "color_preview_manager.hpp"
#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_DECLARE_UI(SettingsDialog)

namespace tactile {

class ThemeOptionsContextMenu;

struct SettingsSnapshot final
{
  QString theme;
  QString defaultFormat;
  int tileWidth{};
  int tileHeight{};
  bool useOpenGL{};
  bool embedTilesets{};
  bool generateDefaults{};
  bool readableOutput{};
};

class SettingsDialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit SettingsDialog(QWidget* parent = nullptr);

  ~SettingsDialog() noexcept override;

 signals:
  void S_ReloadTheme();
  void S_ReloadOpenGl(bool enabled);

 private:
  Unique<Ui::SettingsDialog> mUi;
  ThemeOptionsContextMenu* mThemeOptionsContextMenu{};
  ColorPreviewManager* mBasicPreview{};
  ColorPreviewManager* mDisabledPreview{};
  SettingsSnapshot mSnapshot;

  void UpdateGeneralComponents();

  void UpdateExportComponents();

  void UpdateThemeComponents();

  void UpdateThemePreview();

 private slots:
  void FetchCurrentSettings();

  void OnAccept();

  void OnApply();

  void OnRestoreGeneralDefaults();

  void OnRestoreExportDefaults();

  void OnThemeOptionsButtonPressed();

  void OnRenameCurrentTheme();

  void OnDuplicateCurrentTheme();

  void OnImportNewTheme();

  void OnExportCurrentTheme();

  void OnResetCurrentTheme();

  void OnRemoveCurrentTheme();

  void OnCurrentThemeChanged(const QString& name);

  void OnThemeChanged(QPalette::ColorGroup group,
                      QPalette::ColorRole role,
                      const QColor& color);

  void OnBasicColorChanged(QPalette::ColorRole role, const QColor& color);

  void OnDisabledColorChanged(QPalette::ColorRole role, const QColor& color);
};

}  // namespace tactile
