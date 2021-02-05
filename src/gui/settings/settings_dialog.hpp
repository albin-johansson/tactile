#pragma once

#include <QDialog>  // QDialog
#include <QString>  // QString

#include "color_preview_manager.hpp"
#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, settings_dialog)
TACTILE_FORWARD_DECLARE(tactile::gui, theme_options_context_menu)

namespace tactile::gui {

class settings_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit settings_dialog(QWidget* parent = nullptr);

  ~settings_dialog() noexcept override;

 signals:
  void reload_theme();

 private:
  unique<Ui::settings_dialog> m_ui;
  theme_options_context_menu* m_themeOptionsContextMenu;
  color_preview_manager m_basicPreview;
//  color_preview_manager m_disabledPreview;

  QString m_theme;
  QString m_defaultFormat;
  int m_tileWidth{};
  int m_tileHeight{};
  bool m_embedTilesets{};
  bool m_generateDefaults{};
  bool m_readableOutput{};

  void update_general_components();

  void update_export_components();

  void update_theme_components();

  void update_theme_preview();

 private slots:
  void handle_accept();

  void apply();

  void restore_export_defaults();

  void restore_appearance_defaults();

  void fetch_current_settings();

  void pressed_theme_options_button();

  void duplicate_current_theme();

  void import_new_theme();

  void export_current_theme();

  void reset_current_theme();
};

}  // namespace tactile::gui
