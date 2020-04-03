#pragma once
#include <map>

#include "maybe.h"
#include "smart_pointers.h"

namespace tactile {

class TileSheet;

/**
 * The <code>TileSheetManager</code> class is used to manager multiple
 * instances of the <code>TileSheet</code> class.
 *
 * @see TileSheet
 * @since 0.1.0
 */
class TileSheetManager final {
 public:
  TileSheetManager() noexcept;

  ~TileSheetManager() noexcept;

  /**
   * Adds a tile sheet to the manager. This method has no effect if the supplied
   * tile sheet is null.
   *
   * @param id the key that will be associated with the tile sheet.
   * @param sheet the tile sheet that will be added.
   * @return the ID of the added tile sheet; nothing if no tile sheet was added.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> add(Unique<TileSheet>&& sheet) noexcept;

  /**
   * Removes a tile sheet from the manager. This method has no effect if the
   * specified ID isn't used.
   *
   * @param id the key associated with the tile sheet that will be removed.
   * @since 0.1.0
   */
  void remove(int id) noexcept;

  /**
   * Removes all tile sheets from the manager.
   *
   * @since 0.1.0
   */
  void remove_all() noexcept;

  /**
   * Selects the tile sheet associated with the specified ID.
   *
   * @param id the key associated with the tile sheet that will be made
   * active; nothing indicates that no tile sheet should be selected.
   * @since 0.1.0
   */
  void select(Maybe<int> id) noexcept;

  /**
   * Returns the amount of tile sheets handled by the manager.
   *
   * @return the amount of tile sheets handled by the manager.
   * @since 0.1.0
   */
  [[nodiscard]] int sheets() const noexcept
  {
    return static_cast<int>(m_sheets.size());
  }

  /**
   * Indicates whether or not the manager has an active tile sheet.
   *
   * @return true if there is an active tile sheet; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool has_active_tile_sheet() const noexcept
  {
    return m_activeSheet.has_value();
  }

 private:
  Maybe<int> m_activeSheet;
  std::map<int, Unique<TileSheet>> m_sheets;
  int m_nextSheetKey;
};

}  // namespace tactile
