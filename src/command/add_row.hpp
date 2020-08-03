#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

/**
 * @class add_row
 *
 * @brief Represents the action of adding a row to the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile add_row.hpp
 */
class add_row final : public QUndoCommand {
 public:
  /**
   * @brief Creates am `add_row` instance.
   *
   * @param core a pointer to the associated `core` instance, cannot be null.
   *
   * @since 0.1.0
   */
  explicit add_row(model::core* core);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
};

}  // namespace tactile::cmd
