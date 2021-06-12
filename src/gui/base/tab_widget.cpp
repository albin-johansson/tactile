#include "tab_widget.hpp"

#include "preferences.hpp"
#include "tab_bar.hpp"

namespace tactile {
namespace {

inline constexpr auto styling =
    R"(QTabWidget {
         border: 0;
       }

       QTabWidget::pane {
         margin: 0;
       }

       QTabBar {
         border-radius: 0;
         border: none;
       }

       QTabBar::tab:top {
         margin: 1;
         padding: 2px 8px;
         border-bottom: 3px solid gray;
       }

       QTabBar::tab:top:selected {
         border-bottom-color: %1;
       })";

}  // namespace

TabWidget::TabWidget(QWidget* parent) : QTabWidget{parent}
{
  auto* bar = new TabBar{this};
  connect(bar, &TabBar::S_EditedTab, this, &TabWidget::S_EditedTab);
  setTabBar(bar);
  ApplyStylesheet();
}

void TabWidget::ApplyStylesheet()
{
  const auto accent = prefs::AccentColor().Value();
  setStyleSheet(QString{styling}.arg(accent.name(QColor::HexRgb)));
}

void TabWidget::EditTab(const int index)
{
  qobject_cast<TabBar*>(tabBar())->EditTab(index);
}

}  // namespace tactile
