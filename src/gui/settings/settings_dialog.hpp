#pragma once

#include <qdialog.h>
#include <qstring.h>
#include <qwidget.h>

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
  QString m_initialTheme;

 private slots:
  void handle_accept();
};

}  // namespace tactile::gui
