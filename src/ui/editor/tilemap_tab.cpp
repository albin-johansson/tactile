#include "tilemap_tab.hpp"

#include <algorithm>

#include "tilemap_view.hpp"

namespace tactile::ui {

tilemap_tab::tilemap_tab(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);
  connect(this,
          &QTabWidget::tabCloseRequested,
          this,
          &tilemap_tab::handle_tab_close);
}

tilemap_tab::~tilemap_tab() noexcept = default;

void tilemap_tab::handle_tab_close(int index)
{
  emit request_remove_tab(get_pane(index)->id());
  removeTab(index);
}

auto tilemap_tab::add_tile_map_tab(model::core* core,
                                   const QString& title) noexcept -> int
{
  auto newTitle = title;
  newTitle.append(QString::number(m_currentID));

  auto* view = new tilemap_view{core, m_currentID++};
  addTab(view, newTitle);

  connect(
      view, &tilemap_view::request_redraw, this, &tilemap_tab::request_redraw);

  return view->id();
}

void tilemap_tab::remove_tile_map_tab(int id) noexcept
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    if (auto* pane = get_pane(i); pane && pane->id() == id) {
      removeTab(i);
    }
  }
}

void tilemap_tab::center_viewport(int mapWidth, int mapHeight) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_pane(currentIndex()); pane) {
      pane->center_viewport(mapWidth, mapHeight);
    }
  }
}

void tilemap_tab::move_viewport(int dx, int dy) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_pane(currentIndex()); pane) {
      pane->move_viewport(dx, dy);
    }
  }
}

auto tilemap_tab::next_tab_id() const noexcept -> int
{
  return m_currentID + 1;
}

auto tilemap_tab::get_pane(int index) const noexcept -> tilemap_view*
{
  return qobject_cast<tilemap_view*>(widget(index));
}

auto tilemap_tab::active_tab_id() const noexcept -> std::optional<int>
{
  return tab_id(currentIndex());
}

auto tilemap_tab::tab_id(int index) const noexcept -> std::optional<int>
{
  if (auto* pane = get_pane(index); pane) {
    return pane->id();
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile::ui
