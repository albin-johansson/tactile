#pragma once

#include <QImage>
#include <QObject>
#include <QString>
#include <cassert>   // assert
#include <concepts>  // invocable
#include <memory>    // unique_ptr, shared_ptr
#include <optional>  // optional

#include "command_stack.hpp"
#include "map.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * @class map_document
 *
 * @brief Represents a map and a history of changes to the map.
 *
 * @details This class is a wrapper for a map, its associated tilesets and the
 * command history. It provides an interface similar to that of map for those
 * functions that will be handled by commands. Furthermore, this class provides
 * an overloaded `operator->` to obtain a `const` pointer to the internal map
 * instance, to enable access to the internal map.
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
  void resize(row_t nRows, col_t nCols);

  void add_tileset(tileset_id id, std::shared_ptr<tileset> tileset);

  void remove_tileset(tileset_id id, bool notify = true);

  void select_tileset(tileset_id id);

  void set_selection(position topLeft, position bottomRight);

  void select_layer(layer_id id);  // not a command

  void increase_tile_size();  // not a command

  void decrease_tile_size();  // not a command

  void reset_tile_size();  // not a command

  /**
   * @copydoc map::set_tile()
   */
  void set_tile(const position& pos, tile_id id)  // not a command
  {
    m_map->set_tile(pos, id);
  }

  template <std::invocable<tileset_id, const tileset&> T>
  void each_tileset(T&& lambda) const
  {
    for (const auto& [id, tileset] : *m_tilesets) {
      lambda(id, *tileset);
    }
  }

  template <std::invocable<const layer&> T>
  void each_layer(T&& lambda) const
  {
    for (const auto& layer : *m_map) {
      lambda(layer);
    }
  }

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
   * @copydoc map::in_bounds()
   */
  [[nodiscard]] auto in_bounds(const position& pos) const -> bool
  {
    return m_map->in_bounds(pos);
  }

  /**
   * @copydoc map::tile_at()
   */
  [[nodiscard]] auto tile_at(const position& position) const
      -> std::optional<tile_id>
  {
    return m_map->tile_at(position);
  }

  /**
   * @copydoc map::tile_count()
   */
  [[nodiscard]] auto tile_count() const noexcept -> int
  {
    return m_map->tile_count();
  }

  /**
   * @copydoc map::layer_count()
   */
  [[nodiscard]] auto layer_count() const noexcept -> int
  {
    return m_map->layer_count();
  }

  /**
   * @copydoc map::row_count()
   */
  [[nodiscard]] auto row_count() const -> row_t
  {
    return m_map->row_count();
  }

  /**
   * @copydoc map::col_count()
   */
  [[nodiscard]] auto col_count() const -> col_t
  {
    return m_map->col_count();
  }

  /**
   * @copydoc map::width()
   */
  [[nodiscard]] auto width() const -> int
  {
    return m_map->width();
  }

  /**
   * @copydoc map::height()
   */
  [[nodiscard]] auto height() const -> int
  {
    return m_map->height();
  }

  /**
   * @copydoc map::current_tile_size()
   */
  [[nodiscard]] auto current_tile_size() const noexcept -> int
  {
    return m_map->current_tile_size();
  }

  /**
   * @copydoc tileset_manager::current_tileset()
   */
  [[nodiscard]] auto current_tileset() const -> const tileset*
  {
    return m_tilesets->current_tileset();
  }

  [[nodiscard]] auto tilesets() const noexcept -> const tileset_manager*
  {
    return m_tilesets.get();
  }

 signals:
  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

  void added_tileset(tileset_id id);

  void removed_tileset(tileset_id id);

 public slots:
  void add_tileset(const QImage& image,
                   const QString& path,
                   const QString& name,
                   tile_width tileWidth,
                   tile_height tileHeight);

  void ui_removed_tileset(tileset_id id);

 private:
  std::unique_ptr<map> m_map;
  std::unique_ptr<tileset_manager> m_tilesets;
  command_stack* m_commands{};
};

}  // namespace tactile::core
