#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"
#include "map_id.hpp"
#include "map_position.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

class TilesetWidget;

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
