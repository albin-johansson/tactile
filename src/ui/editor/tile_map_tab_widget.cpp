#include "tile_map_tab_widget.h"

#include "editor_tab.h"

namespace tactile {

TileMapTabWidget::TileMapTabWidget(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);

  connect(this, &QTabWidget::tabCloseRequested, this, [this](int index) {
    removeTab(index);

    // TODO send signal to core...

  });
}

TileMapTabWidget::~TileMapTabWidget() noexcept = default;

void TileMapTabWidget::add_tile_map_tab(const QString& title) noexcept
{
  auto* pane = new EditorTab{};
  addTab(pane, title);

  using RP = EditorTab;
  using TMTW = TileMapTabWidget;
  connect(pane, &RP::rp_req_redraw, this, &TMTW::tmtw_req_redraw);
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

EditorTab* TileMapTabWidget::get_pane(int index) const noexcept
{
  return qobject_cast<EditorTab*>(widget(index));
}

}  // namespace tactile
