#include "tileset_dock.hpp"

#include "map_document.hpp"
#include "tileset_widget.hpp"

namespace tactile {

TilesetDock::TilesetDock(QWidget* parent)
    : DockWidget{parent}
    , mWidget{new TilesetWidget{this}}
{
  setObjectName(QStringLiteral(u"TilesetDock"));
  setWindowTitle(tr("Tilesets"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(mWidget);

  // clang-format off
  connect(mWidget, &TilesetWidget::S_AddTileset, this, &TilesetDock::S_AddTileset);
  connect(mWidget, &TilesetWidget::S_SelectTileset, this, &TilesetDock::S_SelectTileset);
  connect(mWidget, &TilesetWidget::S_RemoveTileset, this, &TilesetDock::S_RemoveTileset);
  connect(mWidget, &TilesetWidget::S_RenameTileset, this, &TilesetDock::S_RenameTileset);
  connect(mWidget, &TilesetWidget::S_ShowProperties, this, &TilesetDock::S_ShowProperties);
  connect(mWidget, &TilesetWidget::S_SetTilesetSelection, this, &TilesetDock::S_SetTilesetSelection);
  // clang-format on
}

void TilesetDock::OnAddedMap(const map_id id, const core::MapDocument& document)
{
  document.EachTileset(
      [this, id](const tileset_id tilesetId, const core::Tileset& tileset) {
        OnAddedTileset(id, tilesetId, tileset);
      });
}

void TilesetDock::OnSwitchedMap(const map_id id)
{
  mWidget->OnSwitchedMap(id);
}

void TilesetDock::OnAddedTileset(const map_id map,
                                 const tileset_id id,
                                 const core::Tileset& tileset)
{
  mWidget->OnAddedTileset(map, id, tileset);
}

void TilesetDock::OnRemovedTileset(const tileset_id id)
{
  mWidget->OnRemovedTileset(id);
}

void TilesetDock::OnRenamedTileset(const tileset_id id, const QString& name)
{
  mWidget->OnRenamedTileset(id, name);
}

}  // namespace tactile
