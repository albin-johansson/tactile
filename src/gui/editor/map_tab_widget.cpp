#include "map_tab_widget.hpp"

#include <algorithm>

#include "map_view.hpp"

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
         margin: 0;
         padding: 2px 8px;
         border-bottom: 3px solid gray;
       }

       QTabBar::tab:top:selected {
         border-bottom-color: #5499c7;
       })";

}  // namespace

map_tab_widget::map_tab_widget(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);
  connect(this,
          &QTabWidget::tabCloseRequested,
          this,
          &map_tab_widget::handle_tab_close);
  setStyleSheet(styling);
}

map_tab_widget::~map_tab_widget() noexcept = default;

void map_tab_widget::handle_tab_close(int index)
{
  emit request_remove_tab(get_view(index)->id());  // TODO rename -> past tense
  removeTab(index);
}

void map_tab_widget::theme_changed()
{
  setStyleSheet(styling);
}

void map_tab_widget::redraw()
{
  if (auto* view = current_view()) {
    view->force_redraw();
  }
}

void map_tab_widget::add_map_tab(not_null<core::map*> map,
                                 const QString& title,
                                 map_id id)
{
  auto* view = new map_view{map, id, this};

  connect(view, &map_view::mouse_pressed, this, &map_tab_widget::mouse_pressed);
  connect(view, &map_view::mouse_moved, this, &map_tab_widget::mouse_moved);
  connect(
      view, &map_view::mouse_released, this, &map_tab_widget::mouse_released);

  addTab(view, title + QString::number(id.get()));
}

void map_tab_widget::remove_map_tab(map_id id)
{
  if (auto* pane = view_from_id(id)) {
    removeTab(indexOf(pane));
  }
}

void map_tab_widget::select_tab(map_id id)
{
  if (auto* pane = view_from_id(id)) {
    setCurrentWidget(pane);
  }
}

void map_tab_widget::center_map()
{
  if (auto* pane = current_view()) {
    pane->center_map();
  }
}

void map_tab_widget::move_map(int dx, int dy)
{
  if (auto* pane = current_view()) {
    pane->move_map(dx, dy);
  }
}

auto map_tab_widget::active_tab_id() const -> std::optional<map_id>
{
  return id_from_index(currentIndex());
}

auto map_tab_widget::id_from_index(int index) const -> std::optional<map_id>
{
  if (const auto* pane = get_view(index)) {
    return pane->id();
  } else {
    return std::nullopt;
  }
}

auto map_tab_widget::current_view() -> map_view*
{
  return get_view(currentIndex());
}

auto map_tab_widget::current_view() const -> const map_view*
{
  return get_view(currentIndex());
}

auto map_tab_widget::get_view(int index) -> map_view*
{
  return qobject_cast<map_view*>(widget(index));
}

auto map_tab_widget::get_view(int index) const -> const map_view*
{
  return qobject_cast<map_view*>(widget(index));
}

auto map_tab_widget::view_from_id(map_id id) -> map_view*
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    if (auto* pane = get_view(i); pane && pane->id() == id) {
      return pane;
    }
  }
  return nullptr;
}

}  // namespace tactile::gui
