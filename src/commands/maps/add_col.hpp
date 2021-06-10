#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document);

namespace tactile::cmd {

/**
 * \class AddCol
 *
 * \brief Represents the dialog of adding a column to the active map.
 *
 * \since 0.1.0
 *
 * \headerfile add_col.hpp
 */
class AddCol final : public repeated_command
{
 public:
  /**
   * \brief Creates am `add_col` instance.
   *
   * \param document the associated map document.
   *
   * \since 0.1.0
   */
  explicit AddCol(not_null<core::map_document*> document);

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
