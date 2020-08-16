#include "mouse_tool_widget.hpp"

#include <QButtonGroup>

#include "ui_tool_button_widget.h"

namespace tactile::ui {

mouse_tool_widget::mouse_tool_widget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::ToolButtonBarUI{}}
{
  m_ui->setupUi(this);

  const auto on_pressed = [this](auto* sender, auto handler) noexcept {
    connect(sender, &QPushButton::pressed, this, handler);
  };

  on_pressed(m_ui->stampButton, &mouse_tool_widget::request_enable_stamp);
  on_pressed(m_ui->bucketButton, &mouse_tool_widget::request_enable_bucket);
  on_pressed(m_ui->eraserButton, &mouse_tool_widget::request_enable_eraser);
  on_pressed(m_ui->rectangleButton,
             &mouse_tool_widget::request_enable_rectangle);
  on_pressed(m_ui->findSameButton,
             &mouse_tool_widget::request_enable_find_same);

  m_group = new QButtonGroup{this};
  m_group->setExclusive(true);
  m_group->addButton(m_ui->stampButton);
  m_group->addButton(m_ui->bucketButton);
  m_group->addButton(m_ui->eraserButton);
  m_group->addButton(m_ui->rectangleButton);
  m_group->addButton(m_ui->findSameButton);
}

mouse_tool_widget::~mouse_tool_widget() noexcept
{
  delete m_ui;
}

void mouse_tool_widget::enable_tools() noexcept
{
  set_tools_disabled(false);
}

void mouse_tool_widget::disable_tools() noexcept
{
  set_tools_disabled(true);
}

void mouse_tool_widget::handle_enable_stamp() noexcept
{
  m_ui->stampButton->setChecked(true);
}

void mouse_tool_widget::handle_enable_bucket() noexcept
{
  m_ui->bucketButton->setChecked(true);
}

void mouse_tool_widget::handle_enable_eraser() noexcept
{
  m_ui->eraserButton->setChecked(true);
}

void mouse_tool_widget::handle_enable_rectangle() noexcept
{
  m_ui->rectangleButton->setChecked(true);
}

void mouse_tool_widget::handle_enable_find_same() noexcept
{
  m_ui->findSameButton->setChecked(true);
}

void mouse_tool_widget::set_tools_disabled(bool disabled) noexcept
{
  m_ui->stampButton->setDisabled(disabled);
  m_ui->bucketButton->setDisabled(disabled);
  m_ui->eraserButton->setDisabled(disabled);
  m_ui->rectangleButton->setDisabled(disabled);
  m_ui->findSameButton->setDisabled(disabled);
}

}  // namespace tactile::gui
