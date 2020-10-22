#pragma once

#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QString>
#include <cassert>   // assert
#include <concepts>  // invocable
#include <memory>    // unique_ptr, shared_ptr
#include <optional>  // optional
#include <utility>   // move

#include "command_stack.hpp"
#include "czstring.hpp"
#include "map.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tileset_manager.hpp"
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
   * @brief Creates an empty map document.
   *
   * @param parent the parent object.
   *
   * @since 0.1.0
   */
  explicit map_document(QObject* parent = nullptr);

  /**
   * @brief Creates a map document with a map that contains one layer.
   *
   * @param nRows the initial number of rows in the map.
   * @param nCols the initial number of columns in the map.
   * @param parent the parent object.
   *
   * @throws tactile_error if `nRows` or `nCols` aren't greater than 0.
   *
   * @since 0.1.0
   */
  explicit map_document(row_t nRows, col_t nCols, QObject* parent = nullptr);

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
   * @copydoc tileset_manager::add(tileset_id,std::shared_ptr<tileset>)
   * @signal `added_tileset`
   */
  void add_tileset(tileset_id id, std::shared_ptr<tileset> tileset);

  /**
   * @copydoc tileset_manager::add(std::shared_ptr<tileset> tileset)
   * @signal `added_tileset`
   */
  void add_tileset(std::shared_ptr<tileset> tileset);

  /**
   * @brief Removes a tileset from the document.
   *
   * @param id the ID associated with the tileset that will be removed.
   * @param notify `true` if a signal should be emitted; `false` otherwise.
   *
   * @since 0.1.0
   *
   * @signal `removed_tileset`
   */
  void remove_tileset(tileset_id id, bool notify = true);

  /**
   * @copydoc tileset_manager::select()
   */
  void select_tileset(tileset_id id);

  /**
   * @copydoc tileset_manager::set_selection()
   */
  void set_selection(position topLeft, position bottomRight);

  /**
   * @copydoc map::select_layer()
   * @signal `selected_layer`
   */
  void select_layer(layer_id id);

  /**
   * @copydoc map::add_layer(layer_id id, layer&& layer)
   * @signal `added_layer`
   */
  void add_layer(layer_id id, layer&& layer);

  /**
   * @copybrief map::add_layer()
   * @signal `added_layer`
   */
  void add_layer();

  /**
   * @brief Attempts to remove the currently active layer.
   *
   * @since 0.1.0
   *
   * @signal `removed_layer`
   */
  void remove_active_layer()
  {
    if (const auto id = m_map->active_layer_id(); id) {
      m_map->remove_active_layer();
      emit removed_layer(*id);
    }
  }

  /**
   * @brief Duplicates the layer associated with the specified ID.
   *
   * @pre `id` must be associated with an existing layer.
   *
   * @param id the ID associated with the layer that will be duplicated.
   *
   * @since 0.1.0
   *
   * @signal `added_duplicated_layer`
   */
  void duplicate_layer(layer_id id);

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
   * @copydoc map::set_visibility()
   */
  void set_layer_visibility(layer_id id, bool visible);

  /**
   * @copydoc map::set_opacity()
   */
  void set_layer_opacity(layer_id id, double opacity);

  /**
   * @copydoc map::set_name()
   */
  void set_layer_name(layer_id id, const QString& name);

  /**
   * @copydoc map::move_layer_back()
   * @signal `moved_layer_back`
   */
  void move_layer_back(layer_id id);

  /**
   * @copydoc map::move_layer_forward()
   * @signal `moved_layer_forward`
   */
  void move_layer_forward(layer_id id);

  /**
   * @copydoc map::set_next_layer_id()
   */
  void set_next_layer_id(layer_id id) noexcept
  {
    m_map->set_next_layer_id(id);
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

  [[nodiscard]] auto get_layer(layer_id id) const -> const layer&
  {
    return m_map->get_layer(id);
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

  [[nodiscard]] auto current_layer_id() const noexcept
      -> std::optional<layer_id>
  {
    return m_map->active_layer_id();
  }

 signals:
  void undo_state_updated(bool canUndo);
  void redo_state_updated(bool canRedo);
  void undo_text_updated(const QString& text);
  void redo_text_updated(const QString& text);

  void added_tileset(tileset_id id);
  void removed_tileset(tileset_id id);

  void added_layer(layer_id id, const layer& layer);
  void added_duplicated_layer(layer_id id, const layer& layer);
  void selected_layer(layer_id id, const layer& layer);
  void removed_layer(layer_id id);  // TODO
  void moved_layer_back(layer_id id);
  void moved_layer_forward(layer_id id);

 public slots:
  /**
   * @brief Adds a tileset to the document.
   *
   * @note This function has no effect if the tileset cannot be added.
   *
   * @param image the image that contains the tile images.
   * @param path the file path of the tileset image.
   * @param name the name associated with the tileset.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @since 0.1.0
   *
   * @signal `added_tileset`
   */
  void add_tileset(const QImage& image,
                   const QFileInfo& path,
                   const QString& name,
                   tile_width tileWidth,
                   tile_height tileHeight);

  void ui_removed_tileset(tileset_id id);

 private:
  std::unique_ptr<map> m_map;
  std::unique_ptr<tileset_manager> m_tilesets;
  command_stack* m_commands{};

  void setup();
};

}  // namespace tactile::core
