#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "repeated_map_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

/**
 * \class remove_row
 *
 * \brief Represents the dialog of removing a row from the active map.
 *
 * \since 0.1.0
 *
 * \headerfile remove_row.hpp
 */
class remove_row final : public repeated_map_command
{
 public:
  explicit remove_row(not_null<core::map_document*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::remove_row);
  }
};

}  // namespace tactile::cmd
