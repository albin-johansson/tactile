#include "tool_dock.hpp"

#include <qlayout.h>

namespace tactile::gui {

tool_dock::tool_dock(QWidget* parent)
    : QDockWidget{parent},
      m_widget{new tool_widget{this}}
{
  setObjectName("toolDock");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setWidget(m_widget);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  connect(
      m_widget, &tool_widget::stamp_enabled, this, &tool_dock::stamp_enabled);
  connect(
      m_widget, &tool_widget::bucket_enabled, this, &tool_dock::bucket_enabled);
  connect(
      m_widget, &tool_widget::eraser_enabled, this, &tool_dock::eraser_enabled);
  connect(m_widget,
          &tool_widget::rectangle_enabled,
          this,
          &tool_dock::rectangle_enabled);
  connect(m_widget,
          &tool_widget::find_same_enabled,
          this,
          &tool_dock::find_same_enabled);
}

}  // namespace tactile::gui
