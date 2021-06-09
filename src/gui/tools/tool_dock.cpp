#include "tool_dock.hpp"

#include <QLayout>

#include "tactile_qstring.hpp"

namespace tactile::gui {

tool_dock::tool_dock(QWidget* parent)
    : DockWidget{parent}
    , m_widget{new tool_widget{this}}
{
  setObjectName(TACTILE_QSTRING(u"tool_dock"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(m_widget);

  // clang-format off
  connect(m_widget, &tool_widget::stamp_enabled, this, &tool_dock::enable_stamp);
  connect(m_widget, &tool_widget::bucket_enabled, this, &tool_dock::enable_bucket);
  connect(m_widget, &tool_widget::eraser_enabled, this, &tool_dock::enable_eraser);
  // clang-format on
}

}  // namespace tactile::gui
