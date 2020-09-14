#pragma once

#include <qobject.h>

#include <memory>  // unique_ptr

#include "command_stack.hpp"
#include "map.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * @class map_document
 *
 * @brief Represents a map and a history of changes to the map.
 *
 * @details This class is a wrapper for a map along with an associated command
 * stack. It provides an interface similar to that of map for those functions
 * that will be handled by commands. For the map functions that aren't handled
 * through commands, use the associated map instance directly instead.
 *
 * @see map
 *
 * @since 0.1.0
 *
 * @headerfile map_document.hpp
 */
class map_document final : public QObject
{
  Q_OBJECT

 public:
  /**
   * @brief Creates a map document.
   *
   * @param parent the parent object.
   *
   * @since 0.1.0
   */
  explicit map_document(QObject* parent = nullptr);

  /**
   * @brief Reverts the effects of the most recent command.
   *
   * @note This function has no effect if there is no undoable command.
   *
   * @since 0.1.0
   */
  void undo();

  /**
   * @brief Executes the most recently reverted command.
   *
   * @note This function has no effect if there is no redoable command.
   *
   * @since 0.1.0
   */
  void redo();

  /**
   * @brief Performs a flood-fill at the specified position.
   *
   * @param position the origin position of the flood-fill.
   * @param target the target tile ID that will be replaced.
   * @param replacement the tile ID that will be used instead of `target`.
   *
   * @todo In theory, the target ID should be possible to determine from the
   * origin position.
   *
   * @since 0.1.0
   */
  void flood(const position& position, tile_id target, tile_id replacement);

  /**
   * @brief Adds a stamp sequence to the command stack.
   *
   * @note Stamp sequence commands are not executed when first put onto the
   * command stack, since they expect their effect to have been applied before
   * the command was created.
   *
   * @param oldState the previous state of the tiles affected by the stamp
   * sequence.
   * @param sequence the stamp sequence with the positions and the new tile IDs.
   *
   * @since 0.1.0
   */
  void add_stamp_sequence(vector_map<position, tile_id>&& oldState,
                          vector_map<position, tile_id>&& sequence);

  /**
   * @brief Adds an erase sequence to the command stack.
   *
   * @note Erase sequence commands are not executed when first put onto the
   * command stack, since they expect their effect to have been applied before
   * the command was created.
   *
   * @param oldState the positions of the affected tiles along with their old
   * IDs.
   *
   * @since 0.1.0
   */
  void add_erase_sequence(vector_map<position, tile_id>&& oldState);

  /**
   * @brief Adds a row to the associated map.
   *
   * @note Consecutive add row commands will be merged into one command.
   *
   * @since 0.1.0
   */
  void add_row();

  /**
   * @brief Adds a column to the associated map.
   *
   * @note Consecutive add column commands will be merged into one command.
   *
   * @since 0.1.0
   */
  void add_column();

  /**
   * @brief Removes a row from the associated map.
   *
   * @note Consecutive remove row commands will be merged into one command.
   *
   * @since 0.1.0
   */
  void remove_row();

  /**
   * @brief Removes a column from the associated map.
   *
   * @note Consecutive remove column commands will be merged into one command.
   *
   * @since 0.1.0
   */
  void remove_column();

  /**
   * @brief Resizes the map.
   *
   * @param nRows the new amount of rows in the map.
   * @param nCols the new amount of columns in the map.
   *
   * @since 0.1.0
   */
  void resize(int nRows, int nCols);

  /**
   * @brief Indicates whether or not there is an undoable command.
   *
   * @return `true` if there is an undoable command; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto can_undo() const -> bool;

  /**
   * @brief Indicates whether or not there is an redoable command.
   *
   * @return `true` if there is an redoable command; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto can_redo() const -> bool;

  /**
   * @brief Returns the text associated with the currently undoable command.
   *
   * @return the text associated with the currently undoable command.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto undo_text() const -> QString;

  /**
   * @brief Returns the text associated with the currently redoable command.
   *
   * @return the text associated with the currently redoable command.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto redo_text() const -> QString;

  /**
   * @brief Returns a pointer to the associated tilemap.
   *
   * @details The returned pointer is stable and will never be null during the
   * lifetime of the map document.
   *
   * @return a pointer to the associated tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get() noexcept -> map*
  {
    return m_map.get();
  }

  /**
   * @copydoc get()
   */
  [[nodiscard]] auto get() const noexcept -> const map*
  {
    return m_map.get();
  }

 signals:
  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

 private:
  std::unique_ptr<map> m_map;
  command_stack* m_commands;
};

}  // namespace tactile::core
