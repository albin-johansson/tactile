#pragma once

#include <QWidget>

namespace Ui {

class StampToolbarUI;

}

namespace tactile::ui {

class StampToolbar final : public QWidget {
  Q_OBJECT

 public:
  explicit StampToolbar(QWidget* parent = nullptr);

  ~StampToolbar() noexcept override;

 private:
  Ui::StampToolbarUI* m_ui;
};

}  // namespace tactile::ui
