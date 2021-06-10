#pragma once

#include <QUndoCommand>  // QUndoCommand
#include <map>           // map
#include <utility>       // pair

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, Map)
TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class MapCommand : public QUndoCommand
{
 public:
  using row_range = std::pair<row_t, row_t>;
  using col_range = std::pair<col_t, col_t>;
  using tile_data = std::map<core::Position, tile_id>;
  using layer_data = std::map<layer_id, tile_data>;

  MapCommand(not_null<core::MapDocument*> document, const QString& name);

 protected:
  void RestoreTiles();

  void SaveTiles(row_range rows, col_range cols);

  void ClearCache();

  void Redraw();

  [[nodiscard]] auto GetMap() noexcept -> core::Map&;

  [[nodiscard]] auto TileData(const layer_id id) -> tile_data&
  {
    // default constructs new map if layer wasn't present
    return mLayerData[id];
  }

  [[nodiscard]] auto LayerData() const -> const layer_data&
  {
    return mLayerData;
  }

 private:
  core::MapDocument* mDocument{};
  layer_data mLayerData;
};

}  // namespace tactile::cmd
