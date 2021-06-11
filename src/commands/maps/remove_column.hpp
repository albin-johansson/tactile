#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "repeated_map_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

/**
 * \class RemoveColumn
 *
 * \brief Represents the dialog of removing a column from the active map.
 *
 * \since 0.1.0
 *
 * \headerfile remove_column.hpp
 */
class RemoveColumn final : public RepeatedMapCommand
{
 public:
  explicit RemoveColumn(not_null<core::MapDocument*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::RemoveColumn);
  }
};

}  // namespace tactile::cmd
