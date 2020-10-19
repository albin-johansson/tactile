#pragma once

#include "command_id.hpp"
#include "map.hpp"
#include "position.hpp"
#include "repeated_map_command.hpp"

namespace tactile::cmd {

/**
 * @class remove_row
 *
 * @brief Represents the dialog of removing a row from the active map.
 *
 * @since 0.1.0
 *
 * @headerfile remove_row.hpp
 */
class remove_row final : public repeated_map_command
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
