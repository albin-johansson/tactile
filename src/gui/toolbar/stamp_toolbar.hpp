#pragma once

#include <QWidget>

#include "types.hpp"

namespace Ui {
class stamp_toolbar;
}

namespace tactile::gui {

class stamp_toolbar final : public QWidget
{
  Q_OBJECT

 public:
  explicit stamp_toolbar(QWidget* parent = nullptr);

  ~stamp_toolbar() noexcept override;

 private:
  Ui::stamp_toolbar* m_ui{};
};

}  // namespace tactile::gui
