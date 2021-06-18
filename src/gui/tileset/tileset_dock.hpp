#pragma once

#include "core_fwd.hpp"
#include "dock_widget.hpp"
#include "gui_fwd.hpp"
#include "map_id.hpp"
#include "map_position.hpp"
#include "tactile_declare_ui.hpp"
#include "tileset_id.hpp"
#include "tileset_selection.hpp"

namespace tactile {

class TilesetDock final : public DockWidget
{
  Q_OBJECT

 public:
  explicit TilesetDock(QWidget* parent = nullptr);

 signals:
  void S_AddTileset();
  void S_SelectTileset(tileset_id id);
  void S_RemoveTileset(tileset_id id);
  void S_RenameTileset(tileset_id id, const QString& name);
  void S_ShowProperties(tileset_id id);
  void S_SetTilesetSelection(const core::TilesetSelection& selection);

 public slots:
  void OnAddedMap(map_id id, const core::MapDocument& document);

  void OnSwitchedMap(map_id id);

  void OnAddedTileset(map_id map, tileset_id id, const core::Tileset& tileset);

  void OnRemovedTileset(tileset_id id);

  void OnRenamedTileset(tileset_id id, const QString& name);

 private:
  TilesetWidget* mWidget{};
};

}  // namespace tactile
