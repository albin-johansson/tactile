#pragma once

#include "abstract_command.hpp"

namespace tactile::cmd {

/**
 * @class remove_row
 *
 * @brief Represents the action of removing a row from the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile remove_row.hpp
 */
class remove_row final : public abstract_command {
 public:
  explicit remove_row(not_null<model::tilemap*> map);

  void undo() override;

  void redo() override;
};

}  // namespace tactile::cmd
