#pragma once

#include <QDialog>
#include <QString>
#include <QWidget>

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
  Ui::settings_dialog* m_ui;
  QString m_theme;
  QString m_defaultFormat;
  int m_tileWidth{};
  int m_tileHeight{};
  bool m_embedTilesets{};
  bool m_generateDefaults{};

 private slots:
  void handle_accept();

  void handle_apply(bool checked);

  void fetch_current_settings();
};

}  // namespace tactile::gui
