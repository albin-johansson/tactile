#pragma once

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
class remove_col final : public row_col_command
{
 public:
  explicit remove_col(core::map* map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::remove_column);
  }
};

}  // namespace tactile::cmd
