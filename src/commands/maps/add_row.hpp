#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

/**
 * \class add_row
 *
 * \brief Represents the dialog of adding a row to the active map.
 *
 * \since 0.1.0
 *
 * \headerfile add_row.hpp
 */
class add_row final : public repeated_command
{
 public:
  explicit add_row(not_null<core::map_document*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::add_row);
  }

 private:
  core::map_document* m_document{};
};

}  // namespace tactile::cmd
