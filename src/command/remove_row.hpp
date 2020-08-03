#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

/**
 * @class remove_row
 *
 * @brief Represents the action of removing a row from the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile remove_row.hpp
 */
class remove_row final : public QUndoCommand {
 public:
  /**
   * @brief Creates a `remove_row` instance.
   *
   * @param core a pointer to the associated `core` instance, cannot be null.
   *
   * @since 0.1.0
   */
  explicit remove_row(model::core* core);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
};

}  // namespace tactile::cmd
