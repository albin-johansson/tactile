#include "dynamic_tool_bar_widget.hpp"

#include <QStackedLayout>
#include <QStackedWidget>

#include "stamp_toolbar.hpp"

namespace tactile::ui {

dynamic_tool_bar_widget::dynamic_tool_bar_widget(QWidget* parent)
    : QWidget{parent}, m_layout{std::make_unique<QStackedLayout>(this)}
{
  // TODO add all different state-based widgets

  auto* stacked = new QStackedWidget{};

  m_stampToolbar = new stamp_toolbar{};

  stacked->addWidget(m_stampToolbar);
  m_layout->addWidget(stacked);

  setLayout(m_layout.get());
}

}  // namespace tactile::ui
