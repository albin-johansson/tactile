#pragma once

#include <QUndoCommand>  // QUndoCommand
#include <map>           // map
#include <utility>       // pair

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map)
TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class map_command : public QUndoCommand
{
 public:
  using row_range = std::pair<row_t, row_t>;
  using col_range = std::pair<col_t, col_t>;
  using tile_data_t = std::map<core::position, tile_id>;
  using layer_data_t = std::map<layer_id, tile_data_t>;

  map_command(not_null<core::map_document*> document, const QString& name);

 protected:
  void restore_tiles();

  void save_tiles(row_range rows, col_range cols);

  void clear_cache();

  void redraw();

  [[nodiscard]] auto get_map() noexcept -> core::map&;

  [[nodiscard]] auto tile_data(layer_id id) -> tile_data_t&
  {
    // default constructs new map if layer wasn't present
    return m_layerData[id];
  }

  [[nodiscard]] auto layer_data() const -> const layer_data_t&
  {
    return m_layerData;
  }

 private:
  core::map_document* m_document{};
  layer_data_t m_layerData;
};

}  // namespace tactile::cmd
