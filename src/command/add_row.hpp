#pragma once

#include "command_id.hpp"
#include "row_col_command.hpp"

namespace tactile::cmd {

/**
 * @class add_row
 *
 * @brief Represents the action of adding a row to the active map.
 *
 * @since 0.1.0
 *
 * @headerfile add_row.hpp
 */
class add_row final : public row_col_command
{
 public:
  explicit add_row(core::map* map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::add_row);
  }
};

}  // namespace tactile::cmd
