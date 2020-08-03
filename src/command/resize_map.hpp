#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

/**
 * @class resize_map
 *
 * @brief Represents the action of resizing the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile resize_map.hpp
 */
class resize_map final : public QUndoCommand {
 public:
  /**
   * @brief Creates a `resize_map` instance.
   *
   * @param core a pointer to the associated `core` instance, cannot be null.
   * @param nRows the new number of rows for the active tilemap.
   * @param nCols the new number of columns for the active tilemap.
   *
   * @since 0.1.0
   */
  explicit resize_map(model::core* core, int nRows, int nCols);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
  int m_rows;
  int m_cols;
  int m_oldRows{1};
  int m_oldCols{1};
};

}  // namespace tactile::cmd
