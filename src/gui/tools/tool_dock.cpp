#include "tool_dock.hpp"

#include <QLayout>

namespace tactile::gui {

tool_dock::tool_dock(QWidget* parent)
    : QDockWidget{parent}, m_widget{new mouse_tool_widget{this}}
{
  setObjectName("toolDock");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setWidget(m_widget);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  connect(m_widget,
          &mouse_tool_widget::request_enable_stamp,
          this,
          &tool_dock::stamp_enabled);
  connect(m_widget,
          &mouse_tool_widget::request_enable_bucket,
          this,
          &tool_dock::bucket_enabled);
  connect(m_widget,
          &mouse_tool_widget::request_enable_eraser,
          this,
          &tool_dock::eraser_enabled);
  connect(m_widget,
          &mouse_tool_widget::request_enable_rectangle,
          this,
          &tool_dock::rectangle_enabled);
  connect(m_widget,
          &mouse_tool_widget::request_enable_find_same,
          this,
          &tool_dock::find_same_enabled);
}

}  // namespace tactile::gui
