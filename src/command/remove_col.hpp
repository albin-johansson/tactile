#pragma once

#include "abstract_command.hpp"

namespace tactile::cmd {

/**
 * @class remove_col
 *
 * @brief Represents the action of removing a column from the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile remove_col.hpp
 */
class remove_col final : public abstract_command
{
 public:
  explicit remove_col(not_null<model::tilemap*> map);

  void undo() override;

  void redo() override;
};

}  // namespace tactile::cmd
