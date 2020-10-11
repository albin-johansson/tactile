#pragma once

#include "command_id.hpp"
#include "row_col_command.hpp"

namespace tactile::cmd {

/**
 * @class remove_row
 *
 * @brief Represents the action of removing a row from the active map.
 *
 * @since 0.1.0
 *
 * @headerfile remove_row.hpp
 */
class remove_row final : public row_col_command
{
 public:
  explicit remove_row(core::map* map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::remove_row);
  }
};

}  // namespace tactile::cmd
