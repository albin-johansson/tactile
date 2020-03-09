#include "startup_widget.h"

#include "ui_startup_widget.h"

namespace tactile {

StartupWidget::StartupWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::StartupWidgetUI{}}
{
  m_ui->setupUi(this);
}

StartupWidget::~StartupWidget() noexcept
{
  delete m_ui;
}

}  // namespace tactile
