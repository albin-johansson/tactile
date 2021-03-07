#include "tab_widget.hpp"

#include "preferences.hpp"
#include "tab_bar.hpp"

namespace tactile::gui {
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

tab_widget::tab_widget(QWidget* parent) : QTabWidget{parent}
{
  auto* bar = new tab_bar{this};
  connect(bar, &tab_bar::edited_tab, this, &tab_widget::edited_tab);
  setTabBar(bar);

  // TODO this needs to be reloaded, so subscribe to reload_accent_color signal
  const auto accent = prefs::gfx::accent_color().value();
  setStyleSheet(QString{styling}.arg(accent.name(QColor::HexRgb)));
}

void tab_widget::edit_tab(const int index)
{
  qobject_cast<tab_bar*>(tabBar())->edit_tab(index);
}

}  // namespace tactile::gui
