#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>

#include "core_fwd.hpp"
#include "position.hpp"
#include "tileset.hpp"

namespace tactile::core {

/**
 * @class tileset_model
 *
 * @brief Used to manager multiple instances of the `tileset` class.
 *
 * @see `tileset`
 *
 * @since 0.1.0
 *
 * @headerfile tileset_model.hpp
 */
class tileset_model final
{
 public:
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
    const auto id{m_nextID};
    ++m_nextID;
    m_tilesets.emplace(id, tileset{std::forward<Args>(args)...});
    m_activeID = id;

    qDebug("tileset_model > added tileset with ID: %i", id.get());
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

  void update_selection(position topLeft, position bottomRight);

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

 private:
  std::optional<tileset_id> m_activeID;
  std::unordered_map<tileset_id, tileset> m_tilesets; // TODO small_map
  tileset_id m_nextID{1};
};

}  // namespace tactile::core
