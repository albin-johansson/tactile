#pragma once


#include "command_id.hpp"
#include "remove_row_col.hpp"

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
class remove_col final : public remove_row_col
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
