#pragma once

#include <optional>  // optional
#include <utility>   // forward, move

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
 public:
  using const_iterator = vector_map<tileset_id, tileset>::const_iterator;

  tileset_manager();

  /**
   * @brief Adds a tileset to the manager.
   *
   * @details The added tileset will be made the currently active tileset.
   *
   * @tparam Args the types of the arguments that will be forwarded.
   *
   * @param args the arguments that will be forwarded to a `tileset`
   * constructor.
   *
   * @return the ID of the added tileset.
   *
   * @since 0.1.0
   */
  template <typename... Args>
  [[nodiscard]] auto emplace(Args&&... args) -> tileset_id
  {
    const auto id = m_nextID;
    ++m_nextID;

    tileset tileset{m_nextGlobalTileID, std::forward<Args>(args)...};

    m_nextGlobalTileID = tileset.last_id() + 1_t;

    m_tilesets.emplace(id, std::move(tileset));
    m_activeID = id;

    return id;
  }

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
  vector_map<tileset_id, tileset> m_tilesets;
  tileset_id m_nextID{1};
  tile_id m_nextGlobalTileID{1};
};

}  // namespace tactile::core
