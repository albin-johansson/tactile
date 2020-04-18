#pragma once
#include <map>

#include "maybe.h"
#include "smart_pointers.h"

namespace tactile {

class Tileset;

/**
 * The <code>TilesetManager</code> class is used to manager multiple
 * instances of the <code>Tileset</code> class.
 *
 * @see Tileset
 * @since 0.1.0
 */
class TilesetManager final {
 public:
  TilesetManager() noexcept;

  ~TilesetManager() noexcept;

  /**
   * Adds a tileset to the manager. This method has no effect if the supplied
   * tileset is null.
   *
   * @param id the key that will be associated with the tileset.
   * @param sheet the tileset that will be added.
   * @return the ID of the added tileset; nothing if no tileset was added.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> add(Unique<Tileset>&& sheet) noexcept;

  /**
   * Removes a tileset from the manager. This method has no effect if the
   * specified ID isn't used.
   *
   * @param id the key associated with the tileset that will be removed.
   * @since 0.1.0
   */
  void remove(int id) noexcept;

  /**
   * Removes all tilesets from the manager.
   *
   * @since 0.1.0
   */
  void remove_all() noexcept;

  /**
   * Selects the tileset associated with the specified ID.
   *
   * @param id the key associated with the tileset that will be made
   * active; nothing indicates that no tileset should be selected.
   * @since 0.1.0
   */
  void select(Maybe<int> id) noexcept;

  /**
   * Returns the amount of tilesets handled by the manager.
   *
   * @return the amount of tilesets handled by the manager.
   * @since 0.1.0
   */
  [[nodiscard]] int sheets() const noexcept
  {
    return static_cast<int>(m_sheets.size());
  }

  /**
   * Indicates whether or not the manager has an active tileset.
   *
   * @return true if there is an active tileset; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool has_active_tile_sheet() const noexcept
  {
    return m_activeSheet.has_value();
  }

 private:
  Maybe<int> m_activeSheet;
  std::map<int, Unique<Tileset>> m_sheets;
  int m_nextSheetKey;
};

}  // namespace tactile
