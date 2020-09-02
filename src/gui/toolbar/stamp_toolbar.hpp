#pragma once

#include <qwidget.h>

#include "tactile_types.hpp"

namespace Ui {

class StampToolbarUI;

}

namespace tactile::gui {

class stamp_toolbar final : public QWidget
{
  Q_OBJECT

 public:
  explicit stamp_toolbar(QWidget* parent = nullptr);

  ~stamp_toolbar() noexcept override;

 private:
  owner<Ui::StampToolbarUI*> m_ui;
};

}  // namespace tactile::gui
