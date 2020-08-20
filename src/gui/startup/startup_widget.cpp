#include "startup_widget.hpp"

#include "ui_startup_widget.h"

namespace tactile::gui {

startup_widget::startup_widget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::startup_widget{}}
{
  m_ui->setupUi(this);
}

startup_widget::~startup_widget() noexcept
{
  delete m_ui;
}

}  // namespace tactile::gui
