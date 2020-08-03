#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

/**
 * @class add_col
 *
 * @brief Represents the action of adding a column to the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile add_col.hpp
 */
class add_col final : public QUndoCommand {
 public:
  /**
   * @brief Creates am `add_col` instance.
   *
   * @param core a pointer to the associated `core` instance, cannot be null.
   *
   * @since 0.1.0
   */
  explicit add_col(model::core* core);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
};

}  // namespace tactile::cmd
