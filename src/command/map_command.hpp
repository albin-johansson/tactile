#pragma once

#include <QUndoCommand>
#include <map>      // map
#include <utility>  // pair

#include "map.hpp"
#include "position.hpp"

namespace tactile::cmd {

class map_command : public QUndoCommand
{
 public:
  using row_range = std::pair<core::row_t, core::row_t>;
  using col_range = std::pair<core::col_t, core::col_t>;

  map_command(core::map* map, const QString& name);

 protected:
  void restore_tiles();

  void save_tiles(row_range rows, col_range cols);

  void clear_cache();

  [[nodiscard]] auto get_map() noexcept -> core::map*
  {
    return m_map;
  }

  [[nodiscard]] auto tile_data(layer_id id)
      -> std::map<core::position, tile_id>&
  {
    // default constructs new map if layer wasn't present
    return m_layerData[id];
  }

  [[nodiscard]] decltype(auto) layer_data() const
  {
    return m_layerData;
  }

 private:
  core::map* m_map{};
  std::map<layer_id, std::map<core::position, tile_id>> m_layerData;
};

}  // namespace tactile::cmd
