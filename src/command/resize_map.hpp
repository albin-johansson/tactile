#pragma once

#include "command_id.hpp"
#include "map_command.hpp"

namespace tactile::cmd {

/**
 * @class resize_map
 *
 * @brief Represents the dialog of resizing the active map.
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
  resize_map(core::map* map, row_t nRows, col_t nCols);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::resize_map);
  }

 private:
  row_t m_rows;
  col_t m_cols;
  row_t m_oldRows{1};
  col_t m_oldCols{1};

  /**
   * @brief Indicates whether or not the command will result in some tiles being
   * removed.
   *
   * @details This is meant to be used to determine whether or not any tiles
   * need to be saved before resizing the map.
   *
   * @return `true` if some tiles will be lost; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto lossy_resize() const noexcept -> bool
  {
    return m_oldRows > m_rows || m_oldCols > m_cols;
  }
};

}  // namespace tactile::cmd
