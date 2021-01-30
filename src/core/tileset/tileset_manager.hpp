#pragma once

#include <QPixmap>  // QPixmap
#include <QRect>    // QRect
#include <utility>  // forward, move, pair

#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * \class tileset_manager
 *
 * \brief Handles multiple `tileset` instances.
 *
 * \see `tileset`
 *
 * \since 0.1.0
 *
 * \headerfile tileset_manager.hpp
 */
class tileset_manager final
{
 public:
  using tile_range = std::pair<tile_id, tile_id>;
  using tileset_map = vector_map<tileset_id, shared<tileset>>;
  using const_iterator = tileset_map::const_iterator;

  /**
   * \brief Creates an empty tileset manager.
   *
   * \since 0.1.0
   */
  tileset_manager();

  /**
   * \brief Adds a tileset to the manager.
   *
   * \note The added tileset will be made the active tileset.
   *
   * \pre `id` must not be associated with an existing tileset.
   * \pre `tileset` cannot be null.
   *
   * \param id the ID that will be associated with the tileset.
   * \param tileset the tileset that will be added to the manager, can't be
   * null.
   *
   * \since 0.1.0
   */
  void add(tileset_id id, shared<tileset> tileset);

  /**
   * \brief Adds a tileset to the manager.
   *
   * \note The added tileset will be made the active tileset.
   *
   * \pre `tileset` can't be null.
   *
   * \param tileset the tileset that was added to the manager, can't be null.
   *
   * \return the ID that was associated with the added tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto add(shared<tileset> tileset) -> tileset_id;

  /**
   * \brief Removes a tileset from the manager.
   *
   * \note This method has no effect if the specified ID isn't used.
   *
   * \param id the key associated with the tileset that will be removed.
   *
   * \since 0.1.0
   */
  void remove(tileset_id id) noexcept;

  /**
   * \brief Removes all tilesets from the manager.
   *
   * \since 0.1.0
   */
  void remove_all() noexcept;  // TODO rename to clear

  /**
   * \brief Sets the name of a tileset.
   *
   * \pre `id` must be associated with a tileset.
   *
   * \param id the ID associated with the tileset that will be renamed.
   * \param name the new name of the tileset.
   *
   * \since 0.1.0
   */
  void rename(tileset_id id, const QString& name);

  /**
   * \brief Selects the tileset associated with the specified ID.
   *
   * \pre `id` must be associated with a tileset if it isn't `std::nullopt`.
   *
   * \param id the key associated with the tileset that will be made
   * active; `std::nullopt` indicates that no tileset should be selected.
   *
   * \since 0.1.0
   */
  void select(maybe<tileset_id> id);

  /**
   * \brief Sets the current tileset selection of the active tileset.
   *
   * \param selection the new selection.
   *
   * \since 0.1.0
   */
  [[deprecated]] void set_selection(const tileset::selection& selection);

  /**
   * \brief Increments the next tileset ID value.
   *
   * \since 0.1.0
   */
  void increment_next_tileset_id() noexcept;

  /**
   * \brief Returns the tileset associated with the specified ID.
   *
   * \pre `id` must be associated with a tileset.
   *
   * \param id the Id associated with the desired tileset.
   *
   * \return the tileset associated with the specified ID.
   *
   * \throws out_of_range if there is no tileset associated with the specified
   * ID.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto at(tileset_id id) -> tileset&;

  /**
   * \copydoc at(tileset_id)
   */
  [[nodiscard]] auto at(tileset_id id) const -> const tileset&;

  /**
   * \brief Returns the image associated with the specified tile.
   *
   * \param id the ID of the tile to obtain the image for.
   *
   * \return the image associated with the specified tile.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto image(tile_id id) const -> const QPixmap&;

  /**
   * \brief Returns the source rectangle associated with the specified tile.
   *
   * \param id the ID of the tile to obtain the source rectangle for.
   *
   * \return the source rectangle associated with the tile.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto image_source(tile_id id) const -> QRect;

  /**
   * \brief Returns the range of tile identifiers associated with the specified
   * tileset.
   *
   * \param id the ID of the tileset that will be queried.
   *
   * \return the range of the specified tileset, as [first, last].
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto range_of(tileset_id id) const -> tile_range;

  /**
   * \brief Returns the amount of tilesets handled by the manager.
   *
   * \return the amount of tilesets handled by the manager.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto count() const noexcept -> int;

  /**
   * \brief Indicates whether or not the manager has an active tileset.
   *
   * \return `true` if there is an active tileset; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto has_active_tileset() const noexcept -> bool;

  /**
   * \brief Returns a pointer to the currently active tileset.
   *
   * \warning Don't claim ownership of the returned pointer!
   *
   * \return a pointer to the currently active tileset; null pointer if there
   * was no active tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto current_tileset() const -> const tileset*;

  /**
   * \brief Returns the ID associated with the active tileset.
   *
   * \return the ID associated with the currently active tileset; `std::nullopt`
   * if there is none.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto current_tileset_id() const -> maybe<tileset_id>;

  /**
   * \brief Indicates whether or not any managed tileset contains a tile ID.
   *
   * \param id the tile ID that will be checked.
   *
   * \return `true` if there is a tileset that contains the specified tile ID;
   * `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto contains(tile_id id) const -> bool;

  /**
   * \brief Indicates whether or not the manager contains a tileset.
   *
   * \param id the tileset ID that will be checked.
   *
   * \return `true` if the manager contains the specified tileset; `false`
   * otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto contains(tileset_id id) const -> bool;

  /**
   * \brief Returns a pointer to the tileset associated with the specified ID.
   *
   * \details This function throws if there is no tileset associated with the
   * specified ID.
   *
   * \param id the ID of the desired tileset.
   *
   * \return a shared pointer to the desired tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto get_tileset_pointer(tileset_id id) -> shared<tileset>
  {
    return m_tilesets.at(id);
  }

  /**
   * \brief Returns the ID of the next tileset.
   *
   * \return the ID that will be used for the next tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto next_tileset_id() const noexcept -> tileset_id
  {
    return m_nextId;
  }

  /**
   * \brief Returns the next available global tile ID.
   *
   * \return the next available global tile ID.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto next_global_tile_id() const noexcept -> tile_id
  {
    return m_nextGlobalTileId;
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_tilesets.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_tilesets.end();
  }

 private:
  maybe<tileset_id> m_activeId;
  tileset_map m_tilesets;
  tileset_id m_nextId{1};
  tile_id m_nextGlobalTileId{1};
};

}  // namespace tactile::core
