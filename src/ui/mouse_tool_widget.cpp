#include "mouse_tool_widget.h"

#include <QButtonGroup>

#include "ui_tool_button_widget.h"

namespace tactile {

MouseToolWidget::MouseToolWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::ToolButtonBarUI{}}
{
  m_ui->setupUi(this);

  const auto on_pressed = [this](auto sender, auto handler) noexcept {
    connect(sender, &QPushButton::pressed, this, handler);
  };

  using MTW = MouseToolWidget;
  on_pressed(m_ui->stampButton, &MTW::stamp_enabled);
  on_pressed(m_ui->bucketButton, &MTW::bucket_enabled);
  on_pressed(m_ui->eraserButton, &MTW::eraser_enabled);
  on_pressed(m_ui->rectangleButton, &MTW::rectangle_enabled);
  on_pressed(m_ui->findSameButton, &MTW::find_same_enabled);

  auto* group = new QButtonGroup{};
  group->setExclusive(true);
  group->addButton(m_ui->stampButton);
  group->addButton(m_ui->bucketButton);
  group->addButton(m_ui->eraserButton);
  group->addButton(m_ui->rectangleButton);
  group->addButton(m_ui->findSameButton);
}

MouseToolWidget::~MouseToolWidget() noexcept
{
  delete m_ui;
}

void MouseToolWidget::enable_tools() noexcept
{
  set_tools_disabled(false);
}

void MouseToolWidget::disable_tools() noexcept
{
  set_tools_disabled(true);
}

void MouseToolWidget::enable_stamp() noexcept
{
  m_ui->stampButton->setChecked(true);
}

void MouseToolWidget::enable_bucket() noexcept
{

  m_ui->bucketButton->setChecked(true);
}

void MouseToolWidget::enable_eraser() noexcept
{
  m_ui->eraserButton->setChecked(true);
}

void MouseToolWidget::enable_rectangle() noexcept
{
  m_ui->rectangleButton->setChecked(true);
}

void MouseToolWidget::enable_find_same() noexcept
{
  m_ui->findSameButton->setChecked(true);
}

void MouseToolWidget::set_tools_disabled(bool disabled) noexcept
{
  m_ui->stampButton->setDisabled(disabled);
  m_ui->bucketButton->setDisabled(disabled);
  m_ui->eraserButton->setDisabled(disabled);
  m_ui->rectangleButton->setDisabled(disabled);
  m_ui->findSameButton->setDisabled(disabled);
}

}  // namespace tactile
