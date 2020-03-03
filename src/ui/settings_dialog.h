#pragma once

#include <QDialog>
#include <QWidget>

namespace Ui {

class SettingsUI;

}

namespace tactile {

class SettingsDialog final : public QDialog {
  Q_OBJECT

 public:
  explicit SettingsDialog(QWidget* parent = nullptr);

  ~SettingsDialog() noexcept override;

 private:
  Ui::SettingsUI* m_ui;
};

}  // namespace tactile
