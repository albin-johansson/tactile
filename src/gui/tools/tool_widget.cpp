#include "tool_widget.hpp"

#include <qbuttongroup.h>

#include "ui_tool_widget.h"

namespace tactile::gui {

tool_widget::tool_widget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::tool_widget{}}
{
  m_ui->setupUi(this);

  const auto on_pressed = [this](auto* sender, auto handler) {
    connect(sender, &QPushButton::pressed, this, handler);
  };

  on_pressed(m_ui->stampButton, &tool_widget::stamp_enabled);
  on_pressed(m_ui->bucketButton, &tool_widget::bucket_enabled);
  on_pressed(m_ui->eraserButton, &tool_widget::eraser_enabled);
  on_pressed(m_ui->rectangleButton, &tool_widget::rectangle_enabled);
  on_pressed(m_ui->findSameButton, &tool_widget::find_same_enabled);

  m_group = new QButtonGroup{this};
  m_group->setExclusive(true);
  m_group->addButton(m_ui->stampButton);
  m_group->addButton(m_ui->bucketButton);
  m_group->addButton(m_ui->eraserButton);
  m_group->addButton(m_ui->rectangleButton);
  m_group->addButton(m_ui->findSameButton);
}

tool_widget::~tool_widget() noexcept
{
  delete m_ui;
}

void tool_widget::enable_tools()
{
  set_tools_disabled(false);
}

void tool_widget::disable_tools()
{
  set_tools_disabled(true);
}

void tool_widget::handle_enable_stamp()
{
  m_ui->stampButton->setChecked(true);
  emit stamp_enabled();
}

void tool_widget::handle_enable_bucket()
{
  m_ui->bucketButton->setChecked(true);
  emit bucket_enabled();
}

void tool_widget::handle_enable_eraser()
{
  m_ui->eraserButton->setChecked(true);
  emit eraser_enabled();
}

void tool_widget::handle_enable_rectangle()
{
  m_ui->rectangleButton->setChecked(true);
  emit rectangle_enabled();
}

void tool_widget::handle_enable_find_same()
{
  m_ui->findSameButton->setChecked(true);
  emit find_same_enabled();
}

void tool_widget::set_tools_disabled(bool disabled)
{
  m_ui->stampButton->setDisabled(disabled);
  m_ui->bucketButton->setDisabled(disabled);
  m_ui->eraserButton->setDisabled(disabled);
  m_ui->rectangleButton->setDisabled(disabled);
  m_ui->findSameButton->setDisabled(disabled);
}

}  // namespace tactile::gui
