#pragma once

#include <QFileInfo>  // QFileInfo
#include <QImage>     // QImage
#include <QString>    // QString
#include <concepts>   // invocable

#include "document.hpp"
#include "layer_id.hpp"
#include "map.hpp"
#include "map_position.hpp"
#include "object_id.hpp"
#include "smart_pointers.hpp"
#include "tile_id.hpp"
#include "tileset_id.hpp"
#include "tileset_manager.hpp"

namespace tactile::core {

class ILayer;
class TileLayer;
class ObjectLayer;
class DocumentDelegate;

/**
 * \class MapDocument
 *
 * \brief Represents a map and a history of changes to the map.
 *
 * \details This class is a wrapper for a map, its associated tilesets and the
 * command history, etc.
 *
 * \see map
 *
 * \since 0.1.0
 *
 * \headerfile map_document.hpp
 */
class MapDocument final : public ADocument
{
  Q_OBJECT

 public:
  /**
   * \brief Creates an empty map document.
   *
   * \param parent the parent object.
   *
   * \since 0.1.0
   */
  explicit MapDocument(QObject* parent = nullptr);

  /**
   * \brief Creates a map document with a map that contains one layer.
   *
   * \param nRows the initial number of rows in the map.
   * \param nCols the initial number of columns in the map.
   * \param parent the parent object.
   *
   * \throws TactileError if `nRows` or `nCols` aren't greater than 0.
   *
   * \since 0.1.0
   */
  MapDocument(row_t nRows, col_t nCols, QObject* parent = nullptr);

  ~MapDocument() noexcept override = default;

  /// \name Document API
  /// \{

  void Undo() override;

  void Redo() override;

  void MarkAsClean() override;

  void ResetHistory() override;

  void SetPath(QFileInfo path) override;

  void ResetPropertyContext() override;

  void SetPropertyContext(NotNull<IPropertyManager*> manager) override;

  [[nodiscard]] auto GetPropertyContext() -> IPropertyManager* override;

  [[nodiscard]] auto CanUndo() const -> bool override;

  [[nodiscard]] auto CanRedo() const -> bool override;

  [[nodiscard]] auto IsClean() const -> bool override;

  [[nodiscard]] auto HasPath() const -> bool override;

  [[nodiscard]] auto GetUndoText() const -> QString override;

  [[nodiscard]] auto GetRedoText() const -> QString override;

  [[nodiscard]] auto Path() const -> const QFileInfo& override;

  [[nodiscard]] auto AbsolutePath() const -> QString override;

  /// \} End of document API

  /// \name Property API
  /// \{

  void AddProperty(const QString& name, core::PropertyType type) override;

  void AddProperty(const QString& name,
                   const core::Property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name,
                   const core::Property& property) override;

  void ChangePropertyType(const QString& name,
                          core::PropertyType type) override;

  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const core::Property& override;

  [[nodiscard]] auto GetProperty(const QString& name)
      -> core::Property& override;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool override;

  [[nodiscard]] auto PropertyCount() const noexcept -> int override;

  [[nodiscard]] auto GetProperties() const -> const property_map& override;

  [[nodiscard]] auto GetName() const -> QStringView override;

  /// \} End of property API

  /// \name Layer API
  /// \{

  void SelectLayer(layer_id id);

  void AddLayer(layer_id id, const Shared<ILayer>& layer);

  auto AddTileLayer() -> layer_id;

  auto AddObjectLayer() -> layer_id;

  /**
   * \brief Removes the specified layer from the document.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID of the layer that will be removed.
   *
   * \since 0.1.0
   *
   * \signal `S_RemovedLayer`
   */
  void RemoveLayer(layer_id id);

  auto TakeLayer(layer_id id) -> Shared<ILayer>;

  /**
   * \brief Duplicates the layer associated with the specified ID.
   *
   * \pre `id` must be associated with an existing layer.
   *
   * \param id the ID associated with the layer that will be duplicated.
   *
   * \since 0.1.0
   *
   * \signal `S_AddedDuplicatedLayer`
   */
  void DuplicateLayer(layer_id id);

  void SetLayerVisibility(layer_id id, bool visible);

  void SetLayerOpacity(layer_id id, double opacity);

  void SetLayerName(layer_id id, const QString& name);

  void MoveLayerBack(layer_id id);

  void MoveLayerForward(layer_id id);

  void SetNextLayerId(layer_id id) noexcept;

  void SetNextObjectId(object_id id) noexcept;

  /**
   * \brief Iterates each layer associated with the document.
   *
   * \tparam T the type of the function object.
   *
   * \param callable the function object that will be invoked for each tile
   * layer.
   *
   * \since 0.1.0
   */
  template <std::invocable<layer_id, const Shared<ILayer>&> T>
  void EachLayer(T&& callable) const
  {
    for (const auto& [key, layer] : *mMap)
    {
      callable(key, layer);
    }
  }

  [[nodiscard]] auto GetLayer(layer_id id) -> ILayer*;

  [[nodiscard]] auto GetLayer(layer_id id) const -> const ILayer*;

  [[nodiscard]] auto GetTileLayer(layer_id id) -> TileLayer*;

  [[nodiscard]] auto GetTileLayer(layer_id id) const -> const TileLayer*;

  [[nodiscard]] auto GetObjectLayer(layer_id id) const -> const ObjectLayer*;

  [[nodiscard]] auto LayerCount() const noexcept -> int;

  [[nodiscard]] auto CurrentLayerId() const noexcept -> Maybe<layer_id>;

  [[nodiscard]] auto HasLayer(layer_id id) const -> bool;

  /// \} End of layer API

  /**
   * \brief Performs a Flood-fill at the specified position.
   *
   * \param position the origin position of the Flood-fill.
   * \param replacement the tile ID that will be used instead of the target ID.
   *
   * \since 0.1.0
   */
  void Flood(const MapPosition& position, tile_id replacement);

  /**
   * \brief Adds a stamp sequence to the command stack.
   *
   * \note Stamp sequence commands are not executed when first put onto the
   * command stack, since they expect their effect to have been applied before
   * the command was created.
   *
   * \param oldState the previous state of the tiles affected by the stamp
   * sequence.
   * \param sequence the stamp sequence with the positions and the new tile IDs.
   *
   * \since 0.1.0
   */
  void AddStampSequence(vector_map<MapPosition, tile_id>&& oldState,
                        vector_map<MapPosition, tile_id>&& sequence);

  /**
   * \brief Adds an erase sequence to the command stack.
   *
   * \note Erase sequence commands are not executed when first put onto the
   * command stack, since they expect their effect to have been applied before
   * the command was created.
   *
   * \param oldState the positions of the affected tiles along with their old
   * IDs.
   *
   * \since 0.1.0
   */
  void AddEraseSequence(vector_map<MapPosition, tile_id>&& oldState);

  /**
   * \brief Adds a row to the associated map.
   *
   * \note Consecutive add row commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void AddRow();

  /**
   * \brief Adds a column to the associated map.
   *
   * \note Consecutive add column commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void AddColumn();

  /**
   * \brief Removes a row from the associated map.
   *
   * \note Consecutive remove row commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void RemoveRow();

  /**
   * \brief Removes a column from the associated map.
   *
   * \note Consecutive remove column commands will be merged into one command.
   *
   * \since 0.1.0
   */
  void RemoveColumn();

  /**
   * \brief Resizes the map.
   *
   * \param nRows the new amount of rows in the map.
   * \param nCols the new amount of columns in the map.
   *
   * \since 0.1.0
   */
  void Resize(row_t nRows, col_t nCols);

  /**
   * \brief Adds a tileset to the document.
   *
   * \note This function has no effect if the tileset cannot be added.
   *
   * \param image the image that contains the tile images.
   * \param path the file path of the tileset image.
   * \param name the name associated with the tileset.
   * \param tileWidth the width of the tiles in the tileset.
   * \param tileHeight the height of the tiles in the tileset.
   *
   * \since 0.1.0
   *
   * \signal `S_AddedTileset`
   */
  void AddTileset(const QImage& image,
                  const QFileInfo& path,
                  const QString& name,
                  tile_width tileWidth,
                  tile_height tileHeight);

  void RemoveTileset(tileset_id id);

  void SelectTileset(tileset_id id);

  void SetTilesetSelection(const TilesetSelection& selection);

  void IncreaseTileSize();

  void DecreaseTileSize();

  void ResetTileSize();

  /**
   * \brief Sets the name of the tileset associated with the specified ID.
   *
   * \param id the ID associated with the tileset that will be renamed.
   * \param name the new name of the tileset.
   *
   * \since 0.1.0
   */
  void SetTilesetName(tileset_id id, const QString& name);

  /**
   * \brief Iterates each tileset associated with the document.
   *
   * \tparam T the type of the function object.
   *
   * \param callable the function object that will be invoked for each tileset.
   *
   * \since 0.1.0
   */
  template <std::invocable<tileset_id, const Tileset&> T>
  void EachTileset(T&& callable) const
  {
    for (const auto& [id, tileset] : *mTilesets)
    {
      callable(id, *tileset);
    }
  }

  [[nodiscard]] auto InBounds(const MapPosition& pos) const -> bool;

  [[nodiscard]] auto RowCount() const -> row_t;

  [[nodiscard]] auto ColumnCount() const -> col_t;

  [[nodiscard]] auto Width() const -> int;

  [[nodiscard]] auto Height() const -> int;

  [[nodiscard]] auto CurrentTileSize() const noexcept -> int;

  [[nodiscard]] auto CurrentTileset() const -> const Tileset*;

  /**
   * \brief Returns a pointer to the associated tileset manager.
   *
   * \return a pointer to the associated tileset manager.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetTilesets() const noexcept -> const TilesetManager*;

  [[nodiscard]] auto GetTilesets() noexcept -> TilesetManager*;

  [[nodiscard]] auto GetTileset(tileset_id id) -> Tileset*;

  [[nodiscard]] auto Raw() -> Map&;

  [[nodiscard]] auto Data() const noexcept -> const Map*;

  [[nodiscard]] auto GetDelegate() -> DocumentDelegate&;

 signals:
  void S_Redraw();

  /// \name Undo/Redo signals
  /// \{

  void S_UndoStateUpdated(bool canUndo);
  void S_RedoStateUpdated(bool canRedo);
  void S_UndoTextUpdated(const QString& text);
  void S_RedoTextUpdated(const QString& text);
  void S_CleanChanged(bool clean);

  /// \} End of Undo/Redo signals

  /// \name Tileset signals
  /// \{

  void S_AddedTileset(tileset_id);
  void S_RemovedTileset(tileset_id);
  void S_RenamedTileset(tileset_id, const QString& name);

  /// \} End of tileset signals

  /// \name Layer signals
  /// \{

  void S_AddedLayer(layer_id, const ILayer&);
  void S_AddedDuplicatedLayer(layer_id, const ILayer&);
  void S_SelectedLayer(layer_id, const ILayer&);
  void S_RemovedLayer(layer_id);
  void S_MovedLayerBack(layer_id);
  void S_MovedLayerForward(layer_id);
  void S_ChangedLayerOpacity(layer_id, double opacity);
  void S_ChangedLayerName(layer_id, const QString&);
  void S_ChangedLayerVisibility(layer_id, bool visible);

  /// \} End of layer signals

  /// \name Property signals
  /// \{

  void S_ShowProperties();
  void S_ShowLayerProperties(layer_id id);
  void S_AddedProperty(const QString& name);
  void S_AboutToRemoveProperty(const QString& name);
  void S_UpdatedProperty(const QString& name);
  void S_ChangedPropertyType(const QString& name);
  void S_RenamedProperty(const QString& oldName, const QString& newName);
  void S_UpdatedPropertyContext(IPropertyManager* context, QStringView name);

  /// \} End of property signals

 private:
  Unique<Map> mMap;                    ///< The associated map.
  Unique<TilesetManager> mTilesets;    ///< The associated tilesets.
  Unique<DocumentDelegate> mDelegate;  ///< Delegate for document API.
  int mTileLayerSuffix{1};             ///< Incrementing tile layer suffix.
  int mObjectLayerSuffix{1};           ///< Incrementing object layer suffix.

  void SetUp();
};

}  // namespace tactile::core
