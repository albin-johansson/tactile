#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>

#include "core_fwd.hpp"
#include "tileset.hpp"

namespace tactile::model {

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
class tileset_model final {
 public:
  /**
   * @brief Adds a tileset to the manager.
   *
   * @tparam Args the types of the arguments that will be forwarded.
   *
   * @param id the key that will be associated with the tileset.
   * @param args the arguments that will be forwarded to a `tileset`
   * constructor.
   *
   * @return the ID of the added tileset.
   *
   * @since 0.1.0
   */
  template <typename... Args>
  [[nodiscard]] auto emplace(Args&&... args) -> int
  {
    const auto id = m_nextID++;
    m_tilesets.emplace(id, tileset{std::forward<Args>(args)...});
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
  void remove(int id) noexcept;

  /**
   * @brief Removes all tilesets from the manager.
   *
   * @since 0.1.0
   */
  void remove_all() noexcept;

  /**
   * @brief Selects the tileset associated with the specified ID.
   *
   * @param id the key associated with the tileset that will be made
   * active; `std::nullopt` indicates that no tileset should be selected.
   *
   * @since 0.1.0
   */
  void select(std::optional<int> id) noexcept;

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

 private:
  std::optional<int> m_activeID;
  std::unordered_map<int, tileset> m_tilesets;
  int m_nextID{1};
};

}  // namespace tactile::model
