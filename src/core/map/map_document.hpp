#pragma once

#include <QFileInfo>  // QFileInfo
#include <QImage>     // QImage
#include <QString>    // QString
#include <concepts>   // invocable

#include "document.hpp"
#include "layer_id.hpp"
#include "map.hpp"
#include "object_id.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"
#include "tile_id.hpp"
#include "tileset_id.hpp"
#include "tileset_manager.hpp"

namespace tactile::core {

class ILayer;
class TileLayer;
class ObjectLayer;
class document_delegate;

/**
 * \class map_document
 *
 * \brief Represents a map and a history of changes to the map.
 *
 * \details This class is a wrapper for a map, its associated tilesets and the
 * command history, etc.
 *
 * \see map
 *
 * \since 0.1.0
 *
 * \headerfile map_document.hpp
 */
class map_document final : public document
{
  Q_OBJECT

 public:
  /**
   * \brief Creates an empty map document.
   *
   * \param parent the parent object.
   *
   * \since 0.1.0
   */
  explicit map_document(QObject* parent = nullptr);

  ~map_document() noexcept override = default;

  /**
   * \brief Creates a map document with a map that contains one layer.
   *
   * \param nRows the initial number of rows in the map.
   * \param nCols the initial number of columns in the map.
   * \param parent the parent object.
   *
   * \throws tactile_error if `nRows` or `nCols` aren't greater than 0.
   *
   * \since 0.1.0
   */
  explicit map_document(row_t nRows, col_t nCols, QObject* parent = nullptr);

  /// \name Document API
  /// \{

  void undo() override;

  void redo() override;

  void mark_as_clean() override;

  void reset_history() override;

  void set_path(QFileInfo path) override;

  [[nodiscard]] auto can_undo() const -> bool override;

  [[nodiscard]] auto can_redo() const -> bool override;

  [[nodiscard]] auto is_clean() const -> bool override;

  [[nodiscard]] auto has_path() const -> bool override;

  [[nodiscard]] auto get_undo_text() const -> QString override;

  [[nodiscard]] auto get_redo_text() const -> QString override;

  [[nodiscard]] auto path() const -> const QFileInfo& override;

  [[nodiscard]] auto absolute_path() const -> QString override;

  /// \} End of document API

  /// \name Property API
  /// \{

  void add_property(const QString& name, core::property_type type) override;

  void add_property(const QString& name,
                    const core::property& property) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name,
                    const core::property& property) override;

  void change_property_type(const QString& name,
                            core::property_type type) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const core::property& override;

  [[nodiscard]] auto get_property(const QString& name)
      -> core::property& override;

  [[nodiscard]] auto has_property(const QString& name) const -> bool override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  [[nodiscard]] auto properties() const -> const property_map& override;

  /// \} End of property API

  /// \name Layer API
  /// \{

  /**
   * \copydoc map::select_layer()
   * \signal `selected_layer`
   */
  void select_layer(layer_id id);

  void add_layer(layer_id id, const shared<ILayer>& layer);

  /**
   * \copybrief map::add_tile_layer()
   * \signal `added_layer`
   */
  auto add_tile_layer() -> layer_id;

  auto add_object_layer() -> layer_id;

  /**
   * \brief Removes the specified layer from the document.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID of the layer that will be removed.
   *
   * \since 0.1.0
   *
   * \signal `removed_layer`
   */
  void remove_layer(layer_id id);

  /**
   * \copydoc map::take_layer()
   */
  auto take_layer(layer_id id) -> shared<ILayer>;

  /**
   * \brief Duplicates the layer associated with the specified ID.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID associated with the layer that will be duplicated.
   *
   * \since 0.1.0
   *
   * \signal `added_duplicated_layer`
   */
  void duplicate_layer(layer_id id);

  /**
   * \copydoc map::set_visibility()
   */
  void set_layer_visibility(layer_id id, bool visible);

  /**
   * \copydoc map::set_opacity()
   */
  void set_layer_opacity(layer_id id, double opacity);

  /**
   * \copydoc map::set_name()
   */
  void set_layer_name(layer_id id, const QString& name);

  /**
   * \copydoc map::move_layer_back()
   * \signal `moved_layer_back`
   */
  void move_layer_back(layer_id id);

  /**
   * \copydoc map::move_layer_forward()
   * \signal `moved_layer_forward`
   */
  void move_layer_forward(layer_id id);

  /**
   * \copydoc map::set_next_layer_id()
   */
  void set_next_layer_id(layer_id id) noexcept;

  void set_next_object_id(object_id id) noexcept;

  /**
   * \brief Iterates each layer associated with the document.
   *
   * \tparam T the type of the function object.
   *
   * \param callable the function object that will be invoked for each
   * tile_layer.
   *
   * \since 0.1.0
   */
  template <std::invocable<layer_id, const shared<ILayer>&> T>
  void each_layer(T&& callable) const
  {
    for (const auto& [key, layer] : *m_map)
    {
      callable(key, layer);
    }
  }

  /**
   * \copydoc map::get_layer()
   */
  [[nodiscard]] auto get_layer(layer_id id) -> ILayer*;

  /**
   * \copydoc map::get_layer()
   */
  [[nodiscard]] auto get_layer(layer_id id) const -> const ILayer*;

  [[nodiscard]] auto get_tile_layer(layer_id id) -> TileLayer*;

  [[nodiscard]] auto get_tile_layer(layer_id id) const -> const TileLayer*;

  [[nodiscard]] auto get_object_layer(layer_id id) const -> const ObjectLayer*;

  /**
   * \copydoc map::layer_count()
   */
  [[nodiscard]] auto layer_count() const noexcept -> int;

  /**
   * \copydoc map::active_layer_id()
   */
  [[nodiscard]] auto current_layer_id() const noexcept -> maybe<layer_id>;

  /**
   * \copydoc map::has_layer()
   */
  [[nodiscard]] auto has_layer(layer_id id) const -> bool;

  /// \} End of layer API

  /**
   * \brief Performs a flood-fill at the specified position.
   *
   * \param position the origin position of the flood-fill.
   * \param replacement the tile ID that will be used instead of the target ID.
   *
   * \since 0.1.0
   */
  void flood(const position& position, tile_id replacement);

  /**
   * \brief Adds a stamp sequence to the command stack.
   *
   * \note Stamp sequence commands are not executed when first put onto the
   * command stack, since they expect their effect to have been applied before
   * the command was created.
   *
   * \param oldState the previous state of the tiles affected by the stamp
   * sequence.
   * \param sequence the stamp sequence with the positions and the new tile IDs.
   *
   * \since 0.1.0
   */
  void add_stamp_sequence(vector_map<position, tile_id>&& oldState,
                          vector_map<position, tile_id>&& sequence);

  /**
   * \brief Adds an erase sequence to the command stack.
   *
   * \note Erase sequence commands are not executed when first put onto the
   * command stack, since they expect their effect to have been applied before
   * the command was created.
   *
   * \param oldState the positions of the affected tiles along with their old
   * IDs.
   *
   * \since 0.1.0
   */
  void add_erase_sequence(vector_map<position, tile_id>&& oldState);

  /**
   * \brief Adds a row to the associated map.
   *
   * \note Consecutive add row commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void add_row();

  /**
   * \brief Adds a column to the associated map.
   *
   * \note Consecutive add column commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void add_column();

  /**
   * \brief Removes a row from the associated map.
   *
   * \note Consecutive remove row commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void remove_row();

  /**
   * \brief Removes a column from the associated map.
   *
   * \note Consecutive remove column commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void remove_column();

  /**
   * \brief Resizes the map.
   *
   * \param nRows the new amount of rows in the map.
   * \param nCols the new amount of columns in the map.
   *
   * \since 0.1.0
   */
  void resize(row_t nRows, col_t nCols);

  /**
   * \brief Adds a tileset to the document.
   *
   * \note This function has no effect if the tileset cannot be added.
   *
   * \param image the image that contains the tile images.
   * \param path the file path of the tileset image.
   * \param name the name associated with the tileset.
   * \param tileWidth the width of the tiles in the tileset.
   * \param tileHeight the height of the tiles in the tileset.
   *
   * \since 0.1.0
   *
   * \signal `added_tileset`
   */
  void add_tileset(const QImage& image,
                   const QFileInfo& path,
                   const QString& name,
                   tile_width tileWidth,
                   tile_height tileHeight);

  void remove_tileset(tileset_id id);

  /**
   * \copydoc tileset_manager::select()
   */
  void select_tileset(tileset_id id);

  /**
   * \copydoc tileset_manager::set_selection()
   */
  void set_tileset_selection(const tileset_selection& selection);

  /**
   * \copydoc map::increase_tile_size()
   */
  void increase_tile_size();

  /**
   * \copydoc map::decrease_tile_size()
   */
  void decrease_tile_size();

  /**
   * \copydoc map::reset_tile_size()
   */
  void reset_tile_size();

  /**
   * \brief Sets the name of the tileset associated with the specified ID.
   *
   * \param id the ID associated with the tileset that will be renamed.
   * \param name the new name of the tileset.
   *
   * \since 0.1.0
   */
  void set_tileset_name(tileset_id id, const QString& name);

  /**
   * \brief Iterates each tileset associated with the document.
   *
   * \tparam T the type of the function object.
   *
   * \param callable the function object that will be invoked for each tileset.
   *
   * \since 0.1.0
   */
  template <std::invocable<tileset_id, const tileset&> T>
  void each_tileset(T&& callable) const
  {
    for (const auto& [id, tileset] : *m_tilesets)
    {
      callable(id, *tileset);
    }
  }

  /**
   * \copydoc map::in_bounds()
   */
  [[nodiscard]] auto in_bounds(const position& pos) const -> bool;

  /**
   * \copydoc map::row_count()
   */
  [[nodiscard]] auto row_count() const -> row_t;

  /**
   * \copydoc map::col_count()
   */
  [[nodiscard]] auto col_count() const -> col_t;

  /**
   * \copydoc map::width()
   */
  [[nodiscard]] auto width() const -> int;

  /**
   * \copydoc map::height()
   */
  [[nodiscard]] auto height() const -> int;

  /**
   * \copydoc map::current_tile_size()
   */
  [[nodiscard]] auto current_tile_size() const noexcept -> int;

  /**
   * \copydoc tileset_manager::current_tileset()
   */
  [[nodiscard]] auto current_tileset() const -> const tileset*;

  /**
   * \brief Returns a pointer to the associated tileset manager.
   *
   * \return a pointer to the associated tileset manager.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto tilesets() const noexcept -> const tileset_manager*;

  [[nodiscard]] auto tilesets() noexcept -> tileset_manager*;

  [[nodiscard]] auto raw() -> map&;

  [[nodiscard]] auto get_map() const noexcept -> const map*;

 signals:
  void redraw();

  /// \name Undo/Redo signals
  /// \{

  void undo_state_updated(bool canUndo);
  void redo_state_updated(bool canRedo);
  void undo_text_updated(const QString& text);
  void redo_text_updated(const QString& text);
  void clean_changed(bool clean);

  /// \} End of Undo/Redo signals

  /// \name Tileset signals
  /// \{

  void added_tileset(tileset_id);
  void removed_tileset(tileset_id);
  void renamed_tileset(tileset_id, const QString& name);

  /// \} End of tileset signals

  /// \name Layer signals
  /// \{

  void added_layer(layer_id, const ILayer&);
  void added_duplicated_layer(layer_id, const ILayer&);
  void selected_layer(layer_id, const ILayer&);
  void removed_layer(layer_id);
  void moved_layer_back(layer_id);
  void moved_layer_forward(layer_id);
  void changed_layer_opacity(layer_id, double opacity);
  void changed_layer_name(layer_id, const QString&);
  void changed_layer_visibility(layer_id, bool visible);

  /// \} End of layer signals

  /// \name Property signals
  /// \{

  void show_properties();
  void show_layer_properties(layer_id id);
  void added_property(const QString& name);
  void about_to_remove_property(const QString& name);
  void updated_property(const QString& name);
  void changed_property_type(const QString& name);
  void renamed_property(const QString& oldName, const QString& newName);

  /// \} End of property signals

 private:
  unique<map> m_map;                     ///< The associated map.
  unique<tileset_manager> m_tilesets;    ///< The associated tilesets.
  unique<document_delegate> m_delegate;  ///< Delegate for document API.
  int m_tileLayerSuffix{1};              ///< Incrementing tile layer suffix.
  int m_objectLayerSuffix{1};            ///< Incrementing object layer suffix.

  void setup();
};

}  // namespace tactile::core
