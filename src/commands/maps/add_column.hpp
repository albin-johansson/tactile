#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document);

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
class AddColumn final : public repeated_command
{
 public:
  explicit AddColumn(not_null<core::map_document*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::add_column);
  }

 private:
  core::map_document* mDocument{};
};

}  // namespace tactile::cmd
