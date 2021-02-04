#pragma once

#include <QDialog>  // QDialog
#include <QString>  // QString

#include "smart_pointers.hpp"

namespace Ui {
class settings_dialog;
}

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
};

}  // namespace tactile::gui
