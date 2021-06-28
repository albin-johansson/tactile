#pragma once

#include <centurion.hpp>   // surface
#include <string>          // string
#include <vector_map.hpp>  // vector_map

#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "aliases/shared.hpp"
#include "aliases/tileset_id.hpp"
#include "tileset.hpp"

namespace tactile {

struct TileRange final
{
  tile_id first;
  tile_id last;
};

/**
 * \class TilesetManager
 *
 * \brief Manages multiple `Tileset` instances.
 *
 * \see `Tileset`
 *
 * \headerfile tileset_manager.hpp
 */
class TilesetManager final
{
 public:
  using storage_type = rune::vector_map<tileset_id, Shared<Tileset>>;
  using const_iterator = storage_type::const_iterator;

  TilesetManager();

  /**
   * \brief Adds a tileset to the manager.
   *
   * \details The next tileset ID property will be updated if it clashes with
   * the specified ID. Otherwise, it remains unaffected.
   *
   * \note The added tileset will be made the active tileset.
   *
   * \pre `id` must not be associated with an existing tileset.
   * \pre `tileset` cannot be null.
   *
   * \param id the ID that will be associated with the tileset.
   * \param tileset the tileset that will be added.
   */
  void Add(tileset_id id, Shared<Tileset> tileset);

  /**
   * \brief Adds a tileset to the manager.
   *
   * \note The added tileset will be made the active tileset.
   *
   * \pre `tileset` can't be null.
   *
   * \param tileset the tileset that was added.
   *
   * \return the ID that was associated with the added tileset.
   */
  [[nodiscard]] auto Add(Shared<Tileset> tileset) -> tileset_id;

  /**
   * \brief Removes a tileset from the manager.
   *
   * \pre `id` must not be associated with an existing tileset.
   *
   * \note If the currently active tileset is removed, no other tileset will
   * be made active.
   *
   * \param id the ID associated with the tileset that will be removed.
   */
  void Remove(tileset_id id);

  /// \brief Removes all tilesets from the manager.
  void Clear();

  /**
   * \brief Sets the name of the specified tileset.
   *
   * \pre `id` must be associated with a tileset.
   *
   * \param id the ID associated with the tileset that will be renamed.
   * \param name the new name of the tileset.
   */
  void Rename(tileset_id id, std::string name);

  /**
   * \brief Selects the tileset associated with the specified ID.
   *
   * \pre `id` must be associated with a tileset (unless it's `nothing`).
   *
   * \param id the key associated with the tileset that will be made active;
   * `nothing` indicates that no tileset should be selected.
   */
  void Select(Maybe<tileset_id> id);

  /**
   * \brief Sets the current tileset selection of the active tileset.
   *
   * \pre There must be an active tileset when this function is called.
   *
   * \param selection the new selection.
   */
  void SetSelection(const TilesetSelection& selection);

  /// \brief Increments the next tileset ID value.
  void IncrementNextTilesetId();

  /**
   * \brief Returns the tileset associated with the specified ID.
   *
   * \param id the ID associated with the desired tileset.
   *
   * \return the tileset associated with the specified ID.
   *
   * \throws out_of_range if there is no tileset associated with the ID.
   */
  [[nodiscard]] auto GetRef(tileset_id id) -> Tileset&;

  /// \copydoc Get()
  [[nodiscard]] auto GetRef(tileset_id id) const -> const Tileset&;

  /**
   * \brief Returns a pointer to the tileset associated with the specified ID.
   *
   * \param id the ID of the desired tileset.
   *
   * \return the desired tileset.
   *
   * \throws out_of_range if there is no tileset associated with the ID.
   */
  [[nodiscard]] auto GetPtr(tileset_id id) -> Shared<Tileset>;

  /**
   * \brief Returns the name associated with the specified tileset.
   *
   * \pre `id` must be associated with a tileset.
   *
   * \param id the ID of the tileset to obtain the name of.
   *
   * \return the name of the tileset.
   */
  [[nodiscard]] auto GetName(tileset_id id) -> std::string_view;

  /**
   * \brief Returns the texture identifier associated with the specified tile.
   *
   * \param id the ID of the tile to obtain the image for.
   *
   * \return the texture identifier associated with the specified tile.
   *
   * \throws TactileError if the identifier cannot be found.
   */
  [[nodiscard]] auto GetTexture(tile_id id) const -> uint;

  /**
   * \brief Returns the source rectangle associated with the specified tile.
   *
   * \param id the ID of the tile to obtain the source rectangle for.
   *
   * \return the source rectangle associated with the tile.
   *
   * \throws TactileError if the associated source rectangle cannot be found.
   */
  [[nodiscard]] auto GetImageSource(tile_id id) const -> cen::irect;

  /**
   * \brief Returns the range of tile IDs associated with the specified tileset.
   *
   * \pre `id` must be associated with a tileset.
   *
   * \param id the ID of the tileset that will be queried.
   *
   * \return the tile range of the specified tileset, as [first, last].
   */
  [[nodiscard]] auto GetRange(tileset_id id) const -> TileRange;

  /**
   * \brief Returns the amount of tilesets handled by the manager.
   *
   * \return the amount of tilesets.
   */
  [[nodiscard]] auto GetSize() const -> usize;

  /**
   * \brief Indicates whether or not the manager has an active tileset.
   *
   * \return `true` if there is an active tileset; `false` otherwise.
   */
  [[nodiscard]] auto HasActiveTileset() const -> bool;

  /**
   * \brief Returns a pointer to the currently active tileset.
   *
   * \warning Don't claim ownership of the returned pointer!
   *
   * \return a pointer to the currently active tileset; null if there is none.
   */
  [[nodiscard]] auto GetActiveTileset() const -> const Tileset*;

  /**
   * \brief Returns the ID associated with the active tileset.
   *
   * \return the ID associated with the currently active tileset; `nothing`
   * if there is none.
   */
  [[nodiscard]] auto GetActiveTilesetId() const -> Maybe<tileset_id>;

  /**
   * \brief Indicates whether or not any managed tileset contains the specified
   * tile.
   *
   * \param id the tile ID to look for.
   *
   * \return `true` if there is a tileset that contains the specified tile ID;
   * `false` otherwise.
   */
  [[nodiscard]] auto Contains(tile_id id) const -> bool;

  /**
   * \brief Indicates whether or not the manager contains a tileset.
   *
   * \param id the tileset ID that will be checked.
   *
   * \return `true` if the manager contains the specified tileset; `false`
   * otherwise.
   */
  [[nodiscard]] auto Contains(tileset_id id) const -> bool;

  /// \brief Returns the ID of the next tileset.
  [[nodiscard]] auto GetNextTilesetId() const noexcept -> tileset_id
  {
    return mNextId;
  }

  /// \brief Returns the next available global tile ID.
  [[nodiscard]] auto GetNextGlobalTileId() const noexcept -> tile_id
  {
    return mNextGlobalTileId;
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mTilesets.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mTilesets.end();
  }

 private:
  storage_type mTilesets;
  Maybe<tileset_id> mCurrentTileset;
  tileset_id mNextId{1};
  tile_id mNextGlobalTileId{1};

  void ValidateNextTilesetId();
};

}  // namespace tactile
