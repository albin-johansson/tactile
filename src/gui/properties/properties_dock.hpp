#pragma once

#include "dock_widget.hpp"

namespace tactile::gui {

class properties_widget;

class properties_dock final : public dock_widget
{
 public:
  explicit properties_dock(QWidget* parent = nullptr);

 private:
  properties_widget* m_widget{};
};

}  // namespace tactile::gui
