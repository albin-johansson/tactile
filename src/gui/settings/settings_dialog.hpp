#pragma once

#include <QDialog>  // QDialog
#include <QString>  // QString

#include "color_preview_manager.hpp"
#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, settings_dialog)
TACTILE_FORWARD_DECLARE(tactile::gui, theme_options_context_menu)

namespace tactile::gui {

struct settings_snapshot final
{
  QString theme;
  QString defaultFormat;
  int tileWidth;
  int tileHeight;
  bool useOpenGL;
  bool embedTilesets;
  bool generateDefaults;
  bool readableOutput;
};

class settings_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit settings_dialog(QWidget* parent = nullptr);

  ~settings_dialog() noexcept override;

 signals:
  void reload_theme();
  void reload_opengl(bool enabled);

 private:
  unique<Ui::settings_dialog> m_ui;
  theme_options_context_menu* m_themeOptionsContextMenu;
  color_preview_manager* m_basicPreview;
  color_preview_manager* m_disabledPreview;
  settings_snapshot m_snapshot;

  void update_general_components();

  void update_export_components();

  void update_theme_components();

  void update_theme_preview();

 private slots:
  void handle_accept();

  void apply();

  void restore_general_defaults();

  void restore_export_defaults();

  void fetch_current_settings();

  void pressed_theme_options_button();

  void rename_current_theme();

  void duplicate_current_theme();

  void import_new_theme();

  void export_current_theme();

  void reset_current_theme();

  void remove_current_theme();

  void when_current_theme_changed(const QString& name);

  void theme_changed(QPalette::ColorGroup group,
                     QPalette::ColorRole role,
                     const QColor& color);
};

}  // namespace tactile::gui
