#include "tab_widget.hpp"

#include "tab_bar.hpp"

namespace tactile::gui {

tab_widget::tab_widget(QWidget* parent) : QTabWidget{parent}
{
  auto* bar = new tab_bar{this};
  connect(bar, &tab_bar::edited_tab, this, &tab_widget::edited_tab);
  setTabBar(bar);
}

void tab_widget::edit_tab(const int index)
{
  qobject_cast<tab_bar*>(tabBar())->edit_tab(index);
}

}  // namespace tactile::gui
