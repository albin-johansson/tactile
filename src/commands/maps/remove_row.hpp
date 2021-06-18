#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "map_position.hpp"
#include "not_null.hpp"
#include "repeated_map_command.hpp"
#include "to_underlying.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

/**
 * \class RemoveRow
 *
 * \brief Represents the dialog of removing a row from the active map.
 *
 * \since 0.1.0
 *
 * \headerfile remove_row.hpp
 */
class RemoveRow final : public RepeatedMapCommand
{
 public:
  explicit RemoveRow(NotNull<core::MapDocument*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::RemoveRow);
  }
};

}  // namespace tactile::cmd
