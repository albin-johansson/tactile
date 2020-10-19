#pragma once

#include "command_id.hpp"
#include "map.hpp"
#include "repeated_command.hpp"

namespace tactile::cmd {

/**
 * @class add_row
 *
 * @brief Represents the dialog of adding a row to the active map.
 *
 * @since 0.1.0
 *
 * @headerfile add_row.hpp
 */
class add_row final : public repeated_command
{
 public:
  explicit add_row(core::map* map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::add_row);
  }

 private:
  core::map* m_map{};
};

}  // namespace tactile::cmd
