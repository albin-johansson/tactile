#pragma once

#include <centurion.hpp>  // irect
#include <filesystem>     // path, absolute
#include <string>         // string
#include <unordered_map>  // unordered_map

#include "aliases/col.hpp"
#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "aliases/row.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/unique.hpp"
#include "core/map/map_position.hpp"
#include "core/properties/property_context.hpp"
#include "core/properties/property_delegate.hpp"
#include "tileset_selection.hpp"
#include "utils/texture_info.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/**
 * \class Tileset
 *
 * \brief Represents a collection of tiles in a sprite sheet.
 *
 * \details A tileset is an image that contains a set of tiles that are used to
 * build tilemaps, where all tilesets store their first and last valid tile
 * identifiers.
 *
 * \details Tilesets must be created from images that store their sprites
 * aligned in a grid. However, the tiles don't necessarily have to be be square.
 *
 * \headerfile tileset.hpp
 */
class Tileset final : public IPropertyContext
{
 public:
  /**
   * \brief Creates a tileset.
   *
   * \param firstId the first tile ID associated with the tileset.
   * \param info the information about the texture that contains the tiles.
   * \param tileWidth the width of the tiles in the tileset.
   * \param tileHeight the height of the tiles in the tileset.
   *
   * \throws TactileError if the supplied tile width or height are less than 1.
   */
  Tileset(tile_id firstId, const TextureInfo& info, int tileWidth, int tileHeight);

  /// \name Tileset API
  /// \{

  /**
   * \brief Iterates the current selection.
   *
   * \details This function has no effect if there is no current selection.
   *
   * \tparam T the type of the callable.
   *
   * \param callable the callable that will be invoked for each tile in the
   * selection.
   */
  template <std::invocable<MapPosition> T>
  void VisitSelection(T&& callable) const
  {
    if (mSelection)
    {
      const auto& [topLeft, bottomRight] = *mSelection;

      const auto nRows = 1_row + (bottomRight.GetRow() - topLeft.GetRow());
      const auto nCols = 1_col + (bottomRight.GetColumn() - topLeft.GetColumn());

      for (row_t row{0}; row < nRows; ++row)
      {
        for (col_t col{0}; col < nCols; ++col)
        {
          callable({row, col});
        }
      }
    }
  }

  /**
   * \brief Iterates the currently selected tiles in the tileset.
   *
   * \details This function is for example useful for the stamp tool, which
   * needs to iterate the selected tiles in the tileset, whilst also keeping
   * track of which position in the map needs to be changed.
   *
   * \note This function has no effect if there is no current selection.
   *
   * \tparam T the type of the callable, the first parameter is the map
   * position, the second parameter is the tileset position.
   *
   * \param mapOrigin the origin tile map position.
   * \param callable the callable that will be invoked for each selected tile in
   * the tileset.
   */
  template <std::invocable<const MapPosition&, const MapPosition&> T>
  void VisitSelection(const MapPosition& mapOrigin, T&& callable) const
  {
    if (mSelection)
    {
      const auto& [topLeft, bottomRight] = *mSelection;

      if (topLeft == bottomRight)
      {
        callable(mapOrigin, topLeft);
      }
      else
      {
        const auto diff = bottomRight - topLeft;
        const MapPosition offset{diff.GetRow() / 2_row, diff.GetColumn() / 2_col};

        const auto endRow = 1_row + diff.GetRow();
        const auto endCol = 1_col + diff.GetColumn();
        for (row_t row{0}; row < endRow; ++row)
        {
          for (col_t col{0}; col < endCol; ++col)
          {
            const auto tilePos = mapOrigin.OffsetBy(row, col) - offset;
            const auto tilesetPos = topLeft.OffsetBy(row, col);
            callable(tilePos, tilesetPos);
          }
        }
      }
    }
  }

  /**
   * \brief Sets the current selection in the tileset.
   *
   * \param selection the new selection.
   */
  void SetSelection(const TilesetSelection& selection);

  /// \brief Clears any current selection.
  void ClearSelection() noexcept;

  /**
   * \brief Sets the name of the tileset.
   *
   * \param name the new name of the tileset.
   */
  void SetName(std::string name);

  /**
   * \brief Sets the file path associated with the tileset.
   *
   * \param path the new file path.
   */
  void SetPath(std::filesystem::path path);

  /**
   * \brief Indicates whether or not the tileset contains the specified tile ID.
   *
   * \param id the tile ID that will be checked.
   *
   * \return `true` if the tileset contains the tile ID; `false` otherwise.
   */
  [[nodiscard]] auto Contains(tile_id id) const -> bool;

  /**
   * \brief Indicates whether or not a single tile is selected in the tileset.
   *
   * \return `true` if only a single tile is selected; `false` otherwise.
   */
  [[nodiscard]] auto IsSingleTileSelected() const -> bool;

  /**
   * \brief Returns the ID of the tile at the specified position.
   *
   * \param position the position of the desired tile.
   *
   * \return the ID of the tile at the specified position; `empty` if the
   * position was out-of-bounds.
   */
  [[nodiscard]] auto GetTile(const MapPosition& position) const -> tile_id;

  /**
   * \brief Returns the source rectangle associated with the specified tile.
   *
   * \param id the ID of the tile to obtain the source rectangle of.
   *
   * \return the source rectangle associated with the tile; `nothing` if
   * no source rectangle was found.
   */
  [[nodiscard]] auto GetImageSource(tile_id id) const -> Maybe<cen::irect>;

  /**
   * \brief Returns the texture identifier associated with the tileset.
   *
   * \return the associated texture identifier.
   */
  [[nodiscard]] auto GetTexture() const -> uint
  {
    return mTexture;
  }

  /**
   * \brief Returns the width of the tileset image.
   *
   * \return the width of the tileset image.
   */
  [[nodiscard]] auto GetWidth() const -> int
  {
    return mWidth;
  }

  /**
   * \brief Returns the height of the tileset image.
   *
   * \return the height of the tileset image.
   */
  [[nodiscard]] auto GetHeight() const -> int
  {
    return mHeight;
  }

  /**
   * \brief Returns the width of the tile sprites in the tileset.
   *
   * \note The returned value is at least 1.
   *
   * \return the width of the tile sprites in the tileset.
   */
  [[nodiscard]] auto GetTileWidth() const noexcept -> int
  {
    return mTileWidth;
  }

  /**
   * \brief Returns the height of the tile sprites in the tileset.
   *
   * \note The returned value is at least 1.
   *
   * \return the height of the tile sprites in the tileset.
   */
  [[nodiscard]] auto GetTileHeight() const noexcept -> int
  {
    return mTileHeight;
  }

  /**
   * \brief Returns the current selection in the tileset.
   *
   * \return the current selection in the tileset.
   */
  [[nodiscard]] auto GetSelection() const noexcept -> const Maybe<TilesetSelection>&
  {
    return mSelection;
  }

  /**
   * \brief Returns the total number of rows of tiles in the tileset.
   *
   * \return the total number of rows in the tileset.
   */
  [[nodiscard]] auto GetRowCount() const noexcept -> row_t
  {
    return mRowCount;
  }

  /**
   * \brief Returns the total number of columns of tiles in the tileset.
   *
   * \return the total number of columns in the tileset.
   */
  [[nodiscard]] auto GetColumnCount() const noexcept -> col_t
  {
    return mColumnCount;
  }

  /**
   * \brief Returns the tile ID of the first tile in the tileset.
   *
   * \details The default first tile ID is `1`.
   *
   * \return the ID of the first tile in the tileset.
   */
  [[nodiscard]] auto GetFirstId() const noexcept -> tile_id
  {
    return mFirstId;
  }

  /**
   * \brief Returns the last valid tile ID associated with the tileset.
   *
   * \return the last valid ID associated with the tileset.
   */
  [[nodiscard]] auto GetLastId() const noexcept -> tile_id
  {
    return mLastId;
  }

  /**
   * \brief Returns the amount of tiles in the tileset.
   *
   * \return the amount of tiles in the tileset.
   */
  [[nodiscard]] auto GetTileCount() const noexcept -> int
  {
    return mTileCount;
  }

  /**
   * \brief Returns the file path of the file associated with the tileset.
   *
   * \return the file path associated with the tileset.
   */
  [[nodiscard]] auto GetFilePath() const -> const std::filesystem::path&
  {
    return mPath;
  }

  /**
   * \brief Returns the absolute file path of the file associated with the
   * tileset.
   *
   * \return the absolute file path associated with the tileset.
   */
  [[nodiscard]] auto GetAbsolutePath() const -> std::filesystem::path
  {
    return std::filesystem::absolute(mPath);
  }

  /// \} End of tileset API

  /// \name Property API
  /// \{

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

 private:
  uint mTexture;
  int mWidth;
  int mHeight;
  tile_id mFirstId{1};
  tile_id mLastId;
  Maybe<TilesetSelection> mSelection;
  std::unordered_map<tile_id, cen::irect> mSourceRects;
  int mTileWidth{};
  int mTileHeight{};
  int mTileCount{};
  row_t mRowCount;
  col_t mColumnCount;
  std::filesystem::path mPath;
  Unique<PropertyDelegate> mProperties;
};

/// \} End of group core

}  // namespace Tactile
