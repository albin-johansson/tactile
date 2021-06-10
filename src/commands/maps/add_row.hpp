#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

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
  explicit AddRow(not_null<core::MapDocument*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::add_row);
  }

 private:
  core::MapDocument* mDocument{};
};

}  // namespace tactile::cmd
