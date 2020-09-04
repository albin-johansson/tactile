#pragma once

#include "abstract_command.hpp"

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
class add_row final : public abstract_command
{
 public:
  explicit add_row(not_null<core::map*> map);

  void undo() override;

  void redo() override;
};

}  // namespace tactile::cmd
