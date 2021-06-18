#pragma once

#include "command_id.hpp"
#include "fwd.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"
#include "tactile_declare_ui.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

/**
 * \class AddColumn
 *
 * \brief Represents the dialog of adding a column to the active map.
 *
 * \since 0.1.0
 *
 * \headerfile add_column.hpp
 */
class AddColumn final : public RepeatedCommand
{
 public:
  explicit AddColumn(NotNull<core::MapDocument*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::AddColumn);
  }

 private:
  core::MapDocument* mDocument{};
};

}  // namespace tactile::cmd
