#pragma once

#include "command_id.hpp"
#include "repeated_command.hpp"
#include "map.hpp"

namespace tactile::cmd {

/**
 * @class add_col
 *
 * @brief Represents the dialog of adding a column to the active map.
 *
 * @since 0.1.0
 *
 * @headerfile add_col.hpp
 */
class add_col final : public repeated_command
{
 public:
  /**
   * @brief Creates am `add_col` instance.
   *
   * @param map the associated map.
   *
   * @since 0.1.0
   */
  explicit add_col(core::map* map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::add_column);
  }

 private:
  core::map* m_map{};
};

}  // namespace tactile::cmd
