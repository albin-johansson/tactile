#include "remove_col.hpp"

#include <algorithm>  // copy

#include "algorithm.hpp"

using tactile::core::operator""_row;
using tactile::core::operator""_col;

namespace tactile::cmd {

remove_col::remove_col(core::map* map)
    : QUndoCommand{QStringLiteral(u"Remove Column")},
      m_map{map},
      m_removedCol{m_map->col_count() - 1_col}
{}

void remove_col::undo()
{
  QUndoCommand::undo();

  invoke_n(m_times, [&] { m_map->add_col(); });

  const auto activeLayer = m_map->active_layer_id();

  for (const auto& [layer, data] : m_layerData) {
    m_map->select_layer(layer);
    for (const auto& [pos, tile] : data) {
      m_map->set_tile(pos, tile);
    }
  }

  m_map->select_layer(activeLayer);
}

void remove_col::redo()
{
  QUndoCommand::redo();

  const auto rows = m_map->row_count();
  const auto endCol = m_map->col_count();  // one past last column
  const auto beginCol = endCol - 1_col - core::col_t{m_times};

  m_map->each_layer([&](layer_id id, const core::layer& layer) {
    std::map<core::position, tile_id> data;

    for (auto row = 0_row; row < rows; ++row) {
      for (auto col = beginCol; col < endCol; ++col) {
        const core::position pos{row, col};
        if (const auto tile = layer.tile_at(pos); tile) {
          data.emplace(pos, *tile);
        }
      }
    }

    m_layerData.emplace(id, std::move(data));
  });

  invoke_n(m_times, [&] { m_map->remove_col(); });
}

auto remove_col::mergeWith(const QUndoCommand* other) -> bool
{
  if (other->id() == id()) {
    if (auto* ptr = dynamic_cast<const remove_col*>(other)) {
      m_times += ptr->m_times;

      for (const auto& [layer, data] : ptr->m_layerData) {
        if (m_layerData.contains(layer)) {
          auto& m = m_layerData.at(layer);

          for (const auto& [pos, tile] : data) {
            m.emplace(pos, tile);
          }

        } else {
          m_layerData.emplace(layer, data);
        }
      }

      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
