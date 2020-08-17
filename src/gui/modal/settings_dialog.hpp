#pragma once

#include <QDialog>
#include <QWidget>

namespace Ui {

class SettingsUI;

}

namespace tactile::gui {

class settings_dialog final : public QDialog {
  Q_OBJECT

 public:
  explicit settings_dialog(QWidget* parent = nullptr);

  ~settings_dialog() noexcept override;

 private:
  Ui::SettingsUI* m_ui;

 private slots:
  void handle_accept();
};

}  // namespace tactile::gui
