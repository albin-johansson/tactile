#pragma once

#include <QUndoCommand>
#include <map>      // map
#include <utility>  // pair

#include "map.hpp"
#include "position.hpp"

namespace tactile::cmd {

class remove_row_col : public QUndoCommand
{
 public:
  using row_range = std::pair<core::row_t, core::row_t>;
  using col_range = std::pair<core::col_t, core::col_t>;

  remove_row_col(core::map* map, const QString& name);

  auto mergeWith(const QUndoCommand* other) -> bool final;

  [[nodiscard]] auto id() const -> int override = 0;

 protected:
  void restore_tiles();

  void save_tiles(row_range rows, col_range cols);

  [[nodiscard]] auto layer_data(layer_id id)
      -> std::map<core::position, tile_id>&
  {
    // default constructs new map if layer wasn't present
    return m_layerData[id];
  }

  [[nodiscard]] auto get_map() noexcept -> core::map*
  {
    return m_map;
  }

  [[nodiscard]] auto times() const noexcept -> int
  {
    return m_times;
  }

 private:
  core::map* m_map{};
  std::map<layer_id, std::map<core::position, tile_id>> m_layerData;
  int m_times{1};
};

}  // namespace tactile::cmd
