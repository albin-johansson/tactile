#pragma once
#include <QWidget>

namespace Ui {

class StartupWidgetUI;

}

namespace tactile {

class StartupWidget final : public QWidget {
 public:
  explicit StartupWidget(QWidget* parent = nullptr);

  ~StartupWidget() noexcept override;

 private:
  Ui::StartupWidgetUI* m_ui;
};

}  // namespace tactile
