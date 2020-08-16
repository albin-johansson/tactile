#include "dynamic_tool_bar_widget.hpp"

#include <QStackedLayout>
#include <QStackedWidget>

#include "stamp_toolbar.hpp"

namespace tactile::gui {

dynamic_tool_bar_widget::dynamic_tool_bar_widget(QWidget* parent)
    : QWidget{parent}
{
  // TODO add all different state-based widgets
  setObjectName("dynamic_tool_bar_widget");

  m_layout = new QStackedLayout{};
  m_stampToolbar = new stamp_toolbar{};

  auto* stacked = new QStackedWidget{};
  stacked->addWidget(m_stampToolbar);  // stacked claims ownership of toolbar

  m_layout->addWidget(stacked);

  setLayout(m_layout);
}

}  // namespace tactile::gui
