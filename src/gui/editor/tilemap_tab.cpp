#include "tilemap_tab.hpp"

#include <algorithm>

#include "tilemap_view.hpp"

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

tilemap_tab::tilemap_tab(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);
  connect(this,
          &QTabWidget::tabCloseRequested,
          this,
          &tilemap_tab::handle_tab_close);
  setStyleSheet(styling);
}

tilemap_tab::~tilemap_tab() noexcept = default;

void tilemap_tab::handle_tab_close(int index)
{
  emit request_remove_tab(get_view(index)->id());
  removeTab(index);
}

void tilemap_tab::theme_changed()
{
  setStyleSheet(styling);
}

void tilemap_tab::redraw()
{
  if (auto* view = get_view(currentIndex())) {
    view->force_redraw();
  }
}

void tilemap_tab::add_tile_map_tab(not_null<model::tilemap*> map,
                                   const QString& title,
                                   int id) noexcept
{
  auto newTitle = title;
  newTitle.append(QString::number(id));

  addTab(new tilemap_view{map, id, this}, newTitle);
}

void tilemap_tab::remove_tile_map_tab(int id) noexcept
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    if (auto* pane = get_view(i); pane && pane->id() == id) {
      removeTab(i);
    }
  }
}

void tilemap_tab::select_tab(int id)
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    if (auto* pane = get_view(i); pane && pane->id() == id) {
      setCurrentIndex(i);
    }
  }
}

void tilemap_tab::center_viewport(int mapWidth, int mapHeight) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_view(currentIndex()); pane) {
      pane->center_viewport(mapWidth, mapHeight);
    }
  }
}

void tilemap_tab::move_viewport(int dx, int dy) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_view(currentIndex()); pane) {
      pane->move_viewport(dx, dy);
    }
  }
}

auto tilemap_tab::get_view(int index) const noexcept -> tilemap_view*
{
  return qobject_cast<tilemap_view*>(widget(index));
}

auto tilemap_tab::active_tab_id() const noexcept -> std::optional<int>
{
  return tab_id(currentIndex());
}

auto tilemap_tab::tab_id(int index) const noexcept -> std::optional<int>
{
  if (auto* pane = get_view(index); pane) {
    return pane->id();
  } else {
    return std::nullopt;
  }
}

void tilemap_tab::paintEvent(QPaintEvent* event)
{
  QTabWidget::paintEvent(event);
}

}  // namespace tactile::gui
