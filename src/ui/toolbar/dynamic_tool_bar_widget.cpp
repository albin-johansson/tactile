#include "dynamic_tool_bar_widget.h"

#include <QStackedLayout>
#include <QStackedWidget>

#include "stamp_toolbar.h"

namespace tactile {

DynamicToolBarWidget::DynamicToolBarWidget(QWidget* parent) : QWidget{parent}
{
  // TODO add all different state-based widgets

  m_layout = new QStackedLayout{this};

  auto* stacked = new QStackedWidget{};

  m_stampToolbar = new StampToolbar{};

  stacked->addWidget(m_stampToolbar);
  m_layout->addWidget(stacked);

  setLayout(m_layout);
}

DynamicToolBarWidget::~DynamicToolBarWidget() noexcept
{
  delete m_layout;
}

}  // namespace tactile
