#include "tile_map_tab_widget.hpp"

#include <algorithm>

#include "editor_tab.hpp"

namespace tactile::ui {

tilemap_tab_widget::tilemap_tab_widget(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);
  connect(this, &QTabWidget::tabCloseRequested, this, [this](int index) {
    emit request_remove_tab(get_pane(index)->id());
    removeTab(index);
  });
}

tilemap_tab_widget::~tilemap_tab_widget() noexcept = default;

auto tilemap_tab_widget::add_tile_map_tab(const QString& title) noexcept -> int
{
  static int id = 0;  // serial ID

  auto newTitle = title;
  newTitle.append(QString::number(id));

  auto* pane = new editor_tab{id++};
  addTab(pane, newTitle);

  connect(
      pane, &editor_tab::s_redraw, this, &tilemap_tab_widget::request_redraw);

  return pane->id();
}

void tilemap_tab_widget::remove_tile_map_tab(int id) noexcept
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    if (auto* pane = get_pane(i); pane && pane->id() == id) {
      removeTab(i);
    }
  }
}

void tilemap_tab_widget::center_viewport(int mapWidth, int mapHeight) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_pane(currentIndex()); pane) {
      pane->center_viewport(mapWidth, mapHeight);
    }
  }
}

void tilemap_tab_widget::move_viewport(int dx, int dy) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_pane(currentIndex()); pane) {
      pane->move_viewport(dx, dy);
    }
  }
}

auto tilemap_tab_widget::get_pane(int index) const noexcept -> editor_tab*
{
  return qobject_cast<editor_tab*>(widget(index));
}

auto tilemap_tab_widget::active_tab_id() const noexcept -> std::optional<int>
{
  return tab_id(currentIndex());
}

auto tilemap_tab_widget::tab_id(int index) const noexcept -> std::optional<int>
{
  if (auto* pane = get_pane(index); pane) {
    return pane->id();
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile::ui
