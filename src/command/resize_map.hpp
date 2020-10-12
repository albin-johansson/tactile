#pragma once

#include "command_id.hpp"
#include "map_command.hpp"

namespace tactile::cmd {

/**
 * @class resize_map
 *
 * @brief Represents the action of resizing the active map.
 *
 * @since 0.1.0
 *
 * @headerfile resize_map.hpp
 */
class resize_map final : public map_command
{
 public:
  /**
   * @brief Creates a `resize_map` instance.
   *
   * @param map a pointer to the associated map.
   * @param nRows the new number of rows for the active map.
   * @param nCols the new number of columns for the active map.
   *
   * @since 0.1.0
   */
  resize_map(core::map* map, core::row_t nRows, core::col_t nCols);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::resize_map);
  }

 private:
  core::row_t m_rows;
  core::col_t m_cols;
  core::row_t m_oldRows{1};
  core::col_t m_oldCols{1};
};

}  // namespace tactile::cmd
