#pragma once

#include <QFileInfo>      // QFileInfo
#include <QImage>         // QImage
#include <QPixmap>        // QPixmap
#include <QRect>          // QRect
#include <QString>        // QString
#include <concepts>       // invocable
#include <unordered_map>  // unordered_map

#include "map_position.hpp"
#include "maybe.hpp"
#include "property_manager.hpp"
#include "smart_pointers.hpp"
#include "tile_height.hpp"
#include "tile_id.hpp"
#include "tile_width.hpp"
#include "tileset_selection.hpp"

namespace tactile::core {

/**
 * \class Tileset
 *
 * \brief Represents a collection of tiles in a sprite sheet.
 *
 * \details A tileset is really just an image that contains a set of tiles
 * that are used to build tilemaps, where all tilesets store their first and
 * last valid tile identifiers.
 *
 * \details Tilesets must be created from images that store their sprites
 * aligned in a grid. However, the tiles don't necessarily have to be be square.
 *
 * \since 0.1.0
 *
 * \headerfile tileset.hpp
 */
class Tileset final : public IPropertyManager
{
 public:
  using rect_map = std::unordered_map<tile_id, QRect>;

  /// \name Construction
  /// \{

  /**
   * \brief Creates a tileset.
   *
   * \param firstId the first tile ID associated with the tileset.
   * \param image the image that contains the tile sprites, mustn't be null.
   * \param tileWidth the width of the tiles in the tileset.
   * \param tileHeight the height of the tiles in the tileset.
   *
   * \throws TactileError if the supplied image is null.
   * \throws TactileError if the supplied tile width or height are less than 1.
   *
   * \since 0.1.0
   */
  Tileset(tile_id firstId,
          const QImage& image,
          tile_width tileWidth,
          tile_height tileHeight);

  /**
   * \brief Creates a tileset.
   *
   * \param firstID the first tile ID associated with the tileset.
   * \param path the path to the image that contains the tile sprites.
   * \param tileWidth the width of the tiles in the tileset.
   * \param tileHeight the height of the tiles in the tileset.
   *
   * \throws TactileError if the tileset cannot be created.
   * \throws TactileError if the supplied tile width or height are less than 1.
   *
   * \since 0.1.0
   */
  Tileset(tile_id firstID,
          const QString& path,
          tile_width tileWidth,
          tile_height tileHeight);

  /// \} End of construction

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
   *
   * \since 0.1.0
   */
  template <std::invocable<MapPosition> T>
  void VisitSelection(T&& callable) const
  {
    if (mSelection)
    {
      const auto& [topLeft, bottomRight] = *mSelection;

      const auto nRows = 1_row + (bottomRight.Row() - topLeft.Row());
      const auto nCols = 1_col + (bottomRight.Column() - topLeft.Column());

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
   *
   * \since 0.1.0
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
        const MapPosition offset{diff.Row() / 2_row, diff.Column() / 2_col};

        const auto endRow = 1_row + diff.Row();
        const auto endCol = 1_col + diff.Column();
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
   *
   * \since 0.1.0
   */
  void SetSelection(const TilesetSelection& selection);

  /**
   * \brief Clears any current selection.
   *
   * \since 0.1.0
   */
  void ClearSelection() noexcept;

  /**
   * \brief Sets the name of the tileset.
   *
   * \param name the new name of the tileset.
   *
   * \since 0.1.0
   */
  void SetName(QString name);

  /**
   * \brief Sets the file path associated with the tileset.
   *
   * \param path the file path associated with the tileset.
   *
   * \since 0.1.0
   */
  void SetPath(QFileInfo path);

  /**
   * \brief Indicates whether or not the tileset contains the specified tile ID.
   *
   * \param id the tile ID that will be checked.
   *
   * \return `true` if the tileset contains the tile ID; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Contains(tile_id id) const noexcept -> bool;

  /**
   * \brief Indicates whether or not a single tile is selected in the tileset.
   *
   * \return `true` if only a single tile is selected; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IsSingleTileSelected() const noexcept -> bool;

  /**
   * \brief Returns the ID of the tile at the specified position.
   *
   * \param position the position of the desired tile.
   *
   * \return the ID of the tile at the specified position; `empty` if the
   * position was out-of-bounds.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TileAt(const MapPosition& position) const -> tile_id;

  /**
   * \brief Returns the width of the tileset image.
   *
   * \return the width of the tileset image.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Width() const -> int;

  /**
   * \brief Returns the height of the tileset image.
   *
   * \return the height of the tileset image.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Height() const -> int;

  /**
   * \brief Returns the image source rectangle associated with the specified
   * tile.
   *
   * \param id the ID of the tile to obtain the source rectangle of.
   *
   * \return the source rectangle associated with the tile; `nothing` if
   * no source rectangle was found.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ImageSource(tile_id id) const -> Maybe<QRect>;

  /**
   * \brief Returns the image associated with the tileset.
   *
   * \return the image associated with the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Image() const -> const QPixmap&
  {
    return mImage;
  }

  /**
   * \brief Returns the width of the tile sprites in the tileset.
   *
   * \note The returned value is at least 1.
   *
   * \return the width of the tile sprites in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetTileWidth() const noexcept -> tile_width
  {
    return mTileWidth;
  }

  /**
   * \brief Returns the height of the tile sprites in the tileset.
   *
   * \note The returned value is at least 1.
   *
   * \return the height of the tile sprites in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetTileHeight() const noexcept -> tile_height
  {
    return mTileHeight;
  }

  /**
   * \brief Returns the current selection in the tileset.
   *
   * \return the current selection in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetSelection() const noexcept
      -> const Maybe<TilesetSelection>&
  {
    return mSelection;
  }

  /**
   * \brief Returns the total number of rows of tiles in the tileset.
   *
   * \return the total number of rows in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto RowCount() const noexcept -> row_t
  {
    return mRowCount;
  }

  /**
   * \brief Returns the total number of columns of tiles in the tileset.
   *
   * \return the total number of columns in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ColumnCount() const noexcept -> col_t
  {
    return mColumnCount;
  }

  /**
   * \brief Returns the tile ID of the first tile in the tileset.
   *
   * \details The default first tile ID is `1`.
   *
   * \return the ID of the first tile in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto FirstId() const noexcept -> tile_id
  {
    return mFirstId;
  }

  /**
   * \brief Returns the last valid tile ID associated with the tileset.
   *
   * \return the last valid ID associated with the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto LastId() const noexcept -> tile_id
  {
    return mLastId;
  }

  /**
   * \brief Returns the amount of tiles in the tileset.
   *
   * \return the amount of tiles in the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TileCount() const noexcept -> int
  {
    return mTileCount;
  }

  /**
   * \brief Returns the name associated with the tileset.
   *
   * \return the name of the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto Name() const -> const QString&
  {
    return mName;
  }

  /**
   * \brief Returns the file associated with the tileset.
   *
   * \return the file associated with the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto File() const -> const QFileInfo&
  {
    return mPath;
  }

  /**
   * \brief Returns the file path of the file associated with the tileset.
   *
   * \return the file path associated with the tileset.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto FilePath() const -> QString
  {
    return mPath.filePath();
  }

  /**
   * \brief Returns the absolute file path of the file associated with the
   * tileset.
   *
   * \return the absolute file path associated with the tileset.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AbsolutePath() const -> QString
  {
    return mPath.absoluteFilePath();
  }

  /// \} End of tileset

  /// \name Property API
  /// \{

  void AddProperty(const QString& name, PropertyType type) override;

  void AddProperty(const QString& name, const Property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name, const Property& property) override;

  void ChangePropertyType(const QString& name, PropertyType type) override;

  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperty(const QString& name) -> Property& override;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool override;

  [[nodiscard]] auto PropertyCount() const noexcept -> int override;

  [[nodiscard]] auto GetProperties() const -> const property_map& override;

  [[nodiscard]] auto GetName() const -> QStringView override;

  /// \} End of property API

 private:
  QPixmap mImage;
  tile_id mFirstId{1};
  tile_id mLastId;
  Maybe<TilesetSelection> mSelection;
  rect_map mSourceRects;
  tile_width mTileWidth;
  tile_height mTileHeight;
  row_t mRowCount;
  col_t mColumnCount;
  int mTileCount{};
  QFileInfo mPath;
  QString mName{QStringLiteral(u"Untitled")};
  Unique<IPropertyManager> mProperties;
};

}  // namespace tactile::core
