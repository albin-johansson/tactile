#pragma once

#include <QPixmap>
#include <QRect>
#include <memory>    // shared_ptr
#include <optional>  // optional
#include <utility>   // forward, move, pair

#include "core_fwd.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * @class tileset_manager
 *
 * @brief Handles multiple tilesets.
 *
 * @see `tileset`
 *
 * @since 0.1.0
 *
 * @headerfile tileset_manager.hpp
 */
class tileset_manager final
{
  using tileset_map = vector_map<tileset_id, std::shared_ptr<tileset>>;

 public:
  using const_iterator = tileset_map::const_iterator;

  tileset_manager();

  void add(tileset_id id, std::shared_ptr<tileset> tileset);

  /**
   * @brief Removes a tileset from the manager.
   *
   * @note This method has no effect if the specified ID isn't used.
   *
   * @param id the key associated with the tileset that will be removed.
   *
   * @since 0.1.0
   */
  void remove(tileset_id id) noexcept;

  /**
   * @brief Removes all tilesets from the manager.
   *
   * @since 0.1.0
   */
  void remove_all() noexcept;

  /**
   * @brief Selects the tileset associated with the specified ID.
   *
   * @pre `id` must be associated with a tileset if it isn't `std::nullopt`.
   *
   * @param id the key associated with the tileset that will be made
   * active; `std::nullopt` indicates that no tileset should be selected.
   *
   * @since 0.1.0
   */
  void select(std::optional<tileset_id> id);

  /**
   * @brief Sets the current tileset selection of the active tileset.
   *
   * @param topLeft the top-left corner of the selection.
   * @param bottomRight the bottom-right corner of the selection.
   *
   * @since 0.1.0
   */
  void set_selection(const position& topLeft, const position& bottomRight);

  void increment_next_tileset_id() noexcept;

  [[nodiscard]] auto at(tileset_id id) -> tileset&;

  [[nodiscard]] auto at(tileset_id id) const -> const tileset&;

  /**
   * @brief Returns the image associated with the specified tile.
   *
   * @param id the ID of the tile to obtain the image for.
   *
   * @return the image associated with the specified tile.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto image(tile_id id) const -> const QPixmap&;

  /**
   * @brief Returns the source rectangle associated with the specified tile.
   *
   * @param id the ID of the tile to obtain the source rectangle for.
   *
   * @return the source rectangle associated with the tile.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto image_source(tile_id id) const -> QRect;

  /**
   * @brief Returns the range of tile identifiers associated with the specified
   * tileset.
   *
   * @param id the ID of the tileset that will be queried.
   *
   * @return the range of the specified tileset, as [first, last].
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto range_of(tileset_id id) const
      -> std::pair<tile_id, tile_id>;

  /**
   * @brief Returns the amount of tilesets handled by the manager.
   *
   * @return the amount of tilesets handled by the manager.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto sheets() const noexcept -> int;

  /**
   * @brief Indicates whether or not the manager has an active tileset.
   *
   * @return `true` if there is an active tileset; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_active_tileset() const noexcept -> bool;

  [[nodiscard]] auto current_tileset() const -> const tileset*;

  [[nodiscard]] auto current_tileset_id() const -> std::optional<tileset_id>;

  [[nodiscard]] auto contains(tile_id id) const -> bool;

  [[nodiscard]] auto contains(tileset_id id) const -> bool;

  [[nodiscard]] auto get_ptr(tileset_id id) -> std::shared_ptr<tileset>&
  {
    return m_tilesets.at(id);
  }

  [[nodiscard]] auto next_tileset_id() const noexcept -> tileset_id
  {
    return m_nextID;
  }

  [[nodiscard]] auto next_global_tile_id() const noexcept -> tile_id
  {
    return m_nextGlobalTileID;
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
  std::optional<tileset_id> m_activeID;
  tileset_map m_tilesets;
  tileset_id m_nextID{1};
  tile_id m_nextGlobalTileID{1};
};

}  // namespace tactile::core
