#pragma once

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "map_command.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

/**
 * \class ResizeMap
 *
 * \brief Represents the dialog of resizing the active map.
 *
 * \since 0.1.0
 *
 * \headerfile resize_map.hpp
 */
class ResizeMap final : public MapCommand
{
 public:
  /**
   * \brief Creates a resize map command.
   *
   * \param document a pointer to the associated map document.
   * \param nRows the new number of rows for the active map.
   * \param nCols the new number of columns for the active map.
   *
   * \since 0.1.0
   */
  ResizeMap(NotNull<core::MapDocument*> document, row_t nRows, col_t nCols);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::ResizeMap);
  }

 private:
  row_t mRows;
  col_t mCols;
  row_t mOldRows{1};
  col_t mOldCols{1};

  /**
   * \brief Indicates whether or not the command will result in some tiles being
   * removed.
   *
   * \details This is meant to be used to determine whether or not any tiles
   * need to be saved before resizing the map.
   *
   * \return `true` if some tiles will be lost; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IsLossyResize() const noexcept -> bool
  {
    return mOldRows > mRows || mOldCols > mCols;
  }
};

}  // namespace tactile::cmd
