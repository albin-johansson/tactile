#pragma once

#include "command_id.hpp"
#include "fwd.hpp"
#include "not_null.hpp"
#include "repeated_map_command.hpp"
#include "to_underlying.hpp"

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
  explicit RemoveColumn(NotNull<core::MapDocument*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::RemoveColumn);
  }
};

}  // namespace tactile::cmd
