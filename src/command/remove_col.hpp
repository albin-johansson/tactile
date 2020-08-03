#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

/**
 * @class remove_col
 *
 * @brief Represents the action of removing a column from the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile remove_col.hpp
 */
class remove_col final : public QUndoCommand {
 public:
  /**
   * @brief Creates a `remove_col` instance.
   *
   * @param core a pointer to the associated `core` instance, cannot be null.
   *
   * @since 0.1.0
   */
  explicit remove_col(model::core* core);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
};

}  // namespace tactile::cmd
