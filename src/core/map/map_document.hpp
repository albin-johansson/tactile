#pragma once

#include <QImage>
#include <QObject>
#include <QString>
#include <cassert>   // assert
#include <concepts>  // invocable
#include <memory>    // unique_ptr, shared_ptr
#include <optional>  // optional
#include <utility>   // move

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
 * command history.
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
   * @param replacement the tile ID that will be used instead of the target ID.
   *
   * @since 0.1.0
   */
  void flood(const position& position, tile_id replacement);

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

  /**
   * @brief Adds a tileset to the document.
   *
   * @details This function emits the `added_tileset(tileset_id)` signal.
   *
   * @param id the ID that will be associated with the tileset.
   * @param tileset the tileset that will be added, cannot be null.
   *
   * @since 0.1.0
   */
  void add_tileset(tileset_id id, std::shared_ptr<tileset> tileset);

  /**
   * @brief Removes a tileset from the document.
   *
   * @details This function emits the `removed_tileset(tileset_id)` signal if
   * `notify` is `true`.
   *
   * @param id the ID associated with the tileset that will be removed.
   * @param notify `true` if a signal should be emitted; `false` otherwise.
   *
   * @since 0.1.0
   */
  void remove_tileset(tileset_id id, bool notify = true);

  /**
   * @copydoc tileset_manager::select_tileset(tileset_id)
   */
  void select_tileset(tileset_id id);

  /**
   * @copydoc tileset_manager::set_selection(position,position)
   */
  void set_selection(position topLeft, position bottomRight);

  /**
   * @copydoc tileset_manager::select_layer(layer_id)
   */
  void select_layer(layer_id id);

  /**
   * @copydoc map::add_layer(layer_id,layer&&)
   */
  void add_layer(layer_id id, layer&& layer)
  {
    m_map->add_layer(id, std::move(layer));
  }

  /**
   * @copydoc map::remove_layers()
   */
  void remove_layers()
  {
    m_map->remove_layers();
  }

  /**
   * @copydoc map::increase_tile_size()
   */
  void increase_tile_size();

  /**
   * @copydoc map::decrease_tile_size()
   */
  void decrease_tile_size();

  /**
   * @copydoc map::reset_tile_size()
   */
  void reset_tile_size();

  /**
   * @copydoc map::set_next_layer_id()
   */
  void set_next_layer_id(layer_id id) noexcept
  {
    m_map->set_next_layer_id(id);
  }

  /**
   * @copydoc tileset_manager::increment_next_tileset_id()
   */
  void increment_next_tileset_id() noexcept
  {
    m_tilesets->increment_next_tileset_id();
  }

  /**
   * @copydoc map::set_tile()
   */
  void set_tile(const position& pos, tile_id id)
  {
    m_map->set_tile(pos, id);
  }

  /**
   * @brief Iterates each tileset associated with the document.
   *
   * @tparam T the type of the function object.
   *
   * @param callable the function object that will be invoked for each tileset.
   *
   * @since 0.1.0
   */
  template <std::invocable<tileset_id, const tileset&> T>
  void each_tileset(T&& callable) const
  {
    for (const auto& [id, tileset] : *m_tilesets) {
      callable(id, *tileset);
    }
  }

  /**
   * @brief Iterates each layer associated with the document.
   *
   * @tparam T the type of the function object.
   *
   * @param callable the function object that will be invoked for each layer.
   *
   * @since 0.1.0
   */
  template <std::invocable<layer_id, const layer&> T>
  void each_layer(T&& callable) const
  {
    for (const auto& [key, layer] : *m_map) {
      callable(key, layer);
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
   * @copydoc tileset_manager::next_tileset_id()
   */
  [[nodiscard]] auto next_tileset_id() const -> tileset_id
  {
    return m_tilesets->next_tileset_id();
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
