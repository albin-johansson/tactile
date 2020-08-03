#pragma once

#include <map>
#include <memory>
#include <optional>

#include "core_fwd.hpp"
#include "tileset.hpp"

namespace tactile::model {

/**
 * @class tileset_manager
 *
 * @brief Used to manager multiple instances of the `tileset` class.
 *
 * @see `tileset`
 *
 * @since 0.1.0
 *
 * @headerfile tileset_manager.hpp
 */
class tileset_manager final {
 public:
  [[nodiscard]] static auto unique() -> std::unique_ptr<tileset_manager>;

  /**
   * @brief Adds a tileset to the manager.
   *
   * @note This method has no effect if the supplied tileset is null.
   *
   * @param id the key that will be associated with the tileset.
   * @param sheet the tileset that will be added.
   *
   * @return the ID of the added tileset; `std::nullopt` if no tileset was
   * added.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto add(std::unique_ptr<tileset>&& sheet) noexcept
      -> std::optional<int>;

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
  std::optional<int> m_activeSheet;
  std::map<int, std::unique_ptr<tileset>> m_sheets;
  int m_nextSheetKey{1};
};

}  // namespace tactile
