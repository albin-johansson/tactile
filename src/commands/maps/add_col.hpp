#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "not_null.hpp"
#include "repeated_command.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document);

namespace tactile::cmd {

/**
 * \class add_col
 *
 * \brief Represents the dialog of adding a column to the active map.
 *
 * \since 0.1.0
 *
 * \headerfile add_col.hpp
 */
class add_col final : public repeated_command
{
 public:
  /**
   * \brief Creates am `add_col` instance.
   *
   * \param document the associated map document.
   *
   * \since 0.1.0
   */
  explicit add_col(not_null<core::map_document*> document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::add_column);
  }

 private:
  core::map_document* m_document{};
};

}  // namespace tactile::cmd
