#pragma once

#include "command_id.hpp"
#include "fwd.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

/**
 * \class AddRow
 *
 * \brief Represents the dialog of adding a row to the active map.
 *
 * \since 0.1.0
 *
 * \headerfile add_row.hpp
 */
class AddRow final : public RepeatedCommand
{
 public:
  explicit AddRow(NotNull<core::MapDocument*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::AddRow);
  }

 private:
  core::MapDocument* mDocument{};
};

}  // namespace tactile::cmd
