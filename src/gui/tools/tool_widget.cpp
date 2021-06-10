#include "tool_widget.hpp"

#include "init_ui.hpp"
#include "ui_tool_widget.h"

namespace tactile {

ToolWidget::ToolWidget(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::ToolWidget>(this)}
    , mGroup{new QButtonGroup{this}}
{
  const auto onPressed = [this](auto* sender, auto handler) {
    connect(sender, &QPushButton::pressed, this, handler);
  };

  onPressed(mUi->stampButton, &ToolWidget::S_StampEnabled);
  onPressed(mUi->bucketButton, &ToolWidget::S_BucketEnabled);
  onPressed(mUi->eraserButton, &ToolWidget::S_EraserEnabled);

  mGroup->setExclusive(true);
  mGroup->addButton(mUi->stampButton);
  mGroup->addButton(mUi->bucketButton);
  mGroup->addButton(mUi->eraserButton);
}

ToolWidget::~ToolWidget() noexcept = default;

void ToolWidget::EnableTools()
{
  SetToolsEnabled(true);
}

void ToolWidget::DisableTools()
{
  SetToolsEnabled(false);
}

void ToolWidget::OnEnableStamp()
{
  mUi->stampButton->setChecked(true);
  emit S_StampEnabled();
}

void ToolWidget::OnEnableBucket()
{
  mUi->bucketButton->setChecked(true);
  emit S_BucketEnabled();
}

void ToolWidget::OnEnableEraser()
{
  mUi->eraserButton->setChecked(true);
  emit S_EraserEnabled();
}

void ToolWidget::SetToolsEnabled(const bool enabled)
{
  mUi->stampButton->setEnabled(enabled);
  mUi->bucketButton->setEnabled(enabled);
  mUi->eraserButton->setEnabled(enabled);
}

}  // namespace tactile
