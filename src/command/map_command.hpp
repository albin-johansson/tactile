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
  using row_range = std::pair<row_t, row_t>;
  using col_range = std::pair<col_t, col_t>;
  using tile_data_t = std::map<core::position, tile_id>;
  using layer_data_t = std::map<layer_id, tile_data_t>;

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

  [[nodiscard]] auto layer_data() const -> const layer_data_t&
  {
    return m_layerData;
  }

 private:
  core::map* m_map{};
  layer_data_t m_layerData;
};

}  // namespace tactile::cmd
