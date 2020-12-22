#include "tool_widget.hpp"

#include "init_ui.hpp"
#include "ui_tool_widget.h"

namespace tactile::gui {

tool_widget::tool_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::tool_widget>(this)}
    , m_group{new QButtonGroup{this}}
{
  const auto on_pressed = [this](auto* sender, auto handler) {
    connect(sender, &QPushButton::pressed, this, handler);
  };

  on_pressed(m_ui->stampButton, &tool_widget::stamp_enabled);
  on_pressed(m_ui->bucketButton, &tool_widget::bucket_enabled);
  on_pressed(m_ui->eraserButton, &tool_widget::eraser_enabled);

  m_group->setExclusive(true);
  m_group->addButton(m_ui->stampButton);
  m_group->addButton(m_ui->bucketButton);
  m_group->addButton(m_ui->eraserButton);
}

tool_widget::~tool_widget() noexcept
{
  delete m_ui;
}

void tool_widget::enable_tools()
{
  set_tools_enabled(true);
}

void tool_widget::disable_tools()
{
  set_tools_enabled(false);
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

void tool_widget::set_tools_enabled(const bool enabled)
{
  m_ui->stampButton->setEnabled(enabled);
  m_ui->bucketButton->setEnabled(enabled);
  m_ui->eraserButton->setEnabled(enabled);
}

}  // namespace tactile::gui
