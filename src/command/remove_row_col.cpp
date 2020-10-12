#include "remove_row_col.hpp"

namespace tactile::cmd {

remove_row_col::remove_row_col(core::map* map, const QString& name)
    : map_command{map, name}
{}

auto remove_row_col::mergeWith(const QUndoCommand* other) -> bool
{
  if (id() == other->id()) {
    if (auto* ptr = dynamic_cast<const remove_row_col*>(other)) {
      m_times += ptr->m_times;

      for (const auto& [layer, data] : ptr->layer_data()) {
        auto& tiles = tile_data(layer);
        for (const auto& [pos, tile] : data) {
          tiles.emplace(pos, tile);
        }
      }

      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
