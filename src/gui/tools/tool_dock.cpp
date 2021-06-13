#include "tool_dock.hpp"

#include <QLayout>

#include "tool_widget.hpp"

namespace tactile {

ToolDock::ToolDock(QWidget* parent)
    : DockWidget{parent}
    , mWidget{new ToolWidget{this}}
{
  setObjectName(QStringLiteral(u"ToolDock"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(mWidget);

  // clang-format off
  connect(mWidget, &ToolWidget::S_StampEnabled, this, &ToolDock::S_EnabledStamp);
  connect(mWidget, &ToolWidget::S_BucketEnabled, this, &ToolDock::S_EnabledBucket);
  connect(mWidget, &ToolWidget::S_EraserEnabled, this, &ToolDock::S_EnabledEraser);
  // clang-format on
}

void ToolDock::EnableTools()
{
  mWidget->EnableTools();
}

void ToolDock::DisableTools()
{
  mWidget->DisableTools();
}

void ToolDock::OnEnableStamp()
{
  mWidget->OnEnableStamp();
}

void ToolDock::OnEnableEraser()
{
  mWidget->OnEnableEraser();
}

void ToolDock::OnEnableBucket()
{
  mWidget->OnEnableBucket();
}

}  // namespace tactile
