#pragma once

#include <QUndoCommand>
#include <map>     // map
#include <vector>  // vector

#include "command_id.hpp"
#include "row_col_command.hpp"

namespace tactile::cmd {

/**
 * @class remove_col
 *
 * @brief Represents the action of removing a column from the active map.
 *
 * @since 0.1.0
 *
 * @headerfile remove_col.hpp
 */
class remove_col final : public QUndoCommand
{
 public:
  explicit remove_col(core::map* map);

  void undo() override;

  void redo() override;

  auto mergeWith(const QUndoCommand* other) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::remove_column);
  }

 private:
  core::map* m_map{};
  core::col_t m_removedCol;
  std::map<layer_id, std::map<core::position, tile_id>> m_layerData;
  int m_times{1};
};

}  // namespace tactile::cmd
