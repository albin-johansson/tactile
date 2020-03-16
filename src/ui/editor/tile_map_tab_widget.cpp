#include "tile_map_tab_widget.h"

#include <algorithm>

#include "editor_tab.h"

namespace tactile {

TileMapTabWidget::TileMapTabWidget(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);
  connect(this, &QTabWidget::tabCloseRequested, this, [this](int index) {
    emit tmtw_req_remove_tab(get_pane(index)->id());
    removeTab(index);
  });
}

TileMapTabWidget::~TileMapTabWidget() noexcept = default;

int TileMapTabWidget::add_tile_map_tab(const QString& title) noexcept
{
  static int id = 0;  // serial ID

  auto newTitle = title;
  newTitle.append(QString::number(id));

  auto* pane = new EditorTab{id++};
  addTab(pane, newTitle);

  using ET = EditorTab;
  using TMTW = TileMapTabWidget;
  connect(pane, &ET::rp_req_redraw, this, &TMTW::tmtw_req_redraw);

  return pane->id();
}

void TileMapTabWidget::remove_tile_map_tab(int id) noexcept
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    auto* pane = get_pane(i);
    if (pane && pane->id() == id) {
      removeTab(i);
    }
  }
}

void TileMapTabWidget::center_viewport(int mapWidth, int mapHeight) noexcept
{
  if (count() > 0) {
    auto* pane = get_pane(currentIndex());
    if (pane) {
      pane->center_viewport(mapWidth, mapHeight);
    }
  }
}

void TileMapTabWidget::move_viewport(int dx, int dy) noexcept
{
  if (count() > 0) {
    auto* pane = get_pane(currentIndex());
    if (pane) {
      pane->move_viewport(dx, dy);
    }
  }
}

EditorTab* TileMapTabWidget::get_pane(int index) const noexcept
{
  return qobject_cast<EditorTab*>(widget(index));
}

Maybe<int> TileMapTabWidget::active_tab_id() const noexcept
{
  auto* pane = get_pane(currentIndex());
  if (pane) {
    return pane->id();
  } else {
    return nothing;
  }
}

}  // namespace tactile
