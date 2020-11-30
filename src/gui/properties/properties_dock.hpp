#pragma once

#include "dock_widget.hpp"

namespace tactile::gui {

class properties_dock final : public dock_widget
{
 public:
  explicit properties_dock(QWidget* parent = nullptr);
};

}  // namespace tactile::gui
