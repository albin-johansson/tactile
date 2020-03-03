#pragma once
#include <QWidget>

namespace Ui {

class ToolButtonBarUI;

}

namespace tactile {

class MouseToolWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit MouseToolWidget(QWidget* parent = nullptr);

  ~MouseToolWidget() noexcept override;

 private:
  Ui::ToolButtonBarUI* m_ui;
};

}  // namespace tactile
