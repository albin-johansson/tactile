#include "map_tab_widget.hpp"

#include <QMouseEvent>

#include "map_document.hpp"
#include "tactile_qstring.hpp"

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
         font-size: 12px;
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
  // clang-format off
  connect(this, &QTabWidget::tabCloseRequested, this, &map_tab_widget::handle_tab_close);
  // clang-format on
  setStyleSheet(styling);
}

map_tab_widget::~map_tab_widget() noexcept = default;

void map_tab_widget::handle_tab_close(const int index)
{
  emit ui_remove_map(get_view(index)->id());
  removeTab(index);
}

void map_tab_widget::theme_changed()
{
  setStyleSheet(styling);
}

void map_tab_widget::force_redraw()
{
  if (auto* view = current_view()) {
    view->force_redraw();
  }
}

void map_tab_widget::add_map_tab(core::map_document* map,
                                 const map_id id,
                                 const QString& title)
{
  auto* view = new map_view{map, id, this};

  // clang-format off
  connect(view, &map_view::mouse_pressed, this, &map_tab_widget::mouse_pressed);
  connect(view, &map_view::mouse_moved, this, &map_tab_widget::mouse_moved);
  connect(view, &map_view::mouse_released, this, &map_tab_widget::mouse_released);
  connect(view, &map_view::mouse_entered, this, &map_tab_widget::mouse_entered);
  connect(view, &map_view::mouse_exited, this, &map_tab_widget::mouse_exited);
  connect(view, &map_view::increase_zoom, this, &map_tab_widget::increase_zoom);
  connect(view, &map_view::decrease_zoom, this, &map_tab_widget::decrease_zoom);
  // clang-format on

  if (title == TACTILE_QSTRING(u"map")) {
    addTab(view, title + QString::number(id.get()));
  } else {
    addTab(view, title);
  }
}

void map_tab_widget::remove_map_tab(const map_id id)
{
  if (auto* view = view_from_id(id)) {
    removeTab(indexOf(view));
  }
}

void map_tab_widget::select_tab(const map_id id)
{
  if (auto* view = view_from_id(id)) {
    setCurrentWidget(view);
  }
}

void map_tab_widget::center_map()
{
  if (auto* view = current_view()) {
    view->center_map();
  }
}

void map_tab_widget::move_map(const int dx, const int dy)
{
  if (auto* view = current_view()) {
    view->move_map(dx, dy);
  }
}

void map_tab_widget::enable_stamp_preview(const core::position& position)
{
  if (auto* view = current_view()) {
    view->enable_stamp_preview(position);
  }
}

void map_tab_widget::disable_stamp_preview()
{
  if (auto* view = current_view()) {
    view->disable_stamp_preview();
  }
}

void map_tab_widget::set_active_tab_name(const QString& name)
{
  if (const auto index = currentIndex(); index != -1) {
    setTabText(index, name);
  }
}

auto map_tab_widget::active_tab_id() const -> maybe<map_id>
{
  return id_from_index(currentIndex());
}

auto map_tab_widget::id_from_index(const int index) const -> maybe<map_id>
{
  if (const auto* view = get_view(index)) {
    return view->id();
  } else {
    return std::nullopt;
  }
}

auto map_tab_widget::active_tab_name() const -> maybe<QString>
{
  const auto index = currentIndex();
  if (index != -1) {
    return tabText(index);
  } else {
    return std::nullopt;
  }
}

auto map_tab_widget::current_view() -> map_view*
{
  return get_view(currentIndex());
}

auto map_tab_widget::get_view(const int index) -> map_view*
{
  return qobject_cast<map_view*>(widget(index));
}

auto map_tab_widget::get_view(const int index) const -> const map_view*
{
  return qobject_cast<map_view*>(widget(index));
}

auto map_tab_widget::view_from_id(const map_id id) -> map_view*
{
  const auto amount = count();
  for (auto i = 0; i < amount; ++i) {
    if (auto* pane = get_view(i); pane && pane->id() == id) {
      return pane;
    }
  }
  return nullptr;
}

}  // namespace tactile::gui
