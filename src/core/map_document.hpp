#pragma once

#include <utility> // pair

#include "aliases/ints.hpp"
#include "aliases/unique.hpp"
#include "core/document.hpp"
#include "core/document_delegate.hpp"
#include "core/map/map.hpp"
#include "core/properties/property_delegate.hpp"
#include "core/tileset/tileset_manager.hpp"
#include "core/viewport_info.hpp"
#include "utils/texture_info.hpp"

namespace Tactile {

class MapDocument final : public IDocument
{
 public:
  using TileCache = rune::vector_map<MapPosition, tile_id>;

  MapDocument();

  MapDocument(row_t nRows, col_t nCols, int tileWidth, int tileHeight);

  /// \name Document API
  /// \{

  void Undo() override;

  void Redo() override;

  void MarkAsClean() override;

  void ResetHistory() override;

  void SetPath(std::filesystem::path path) override;

  void SetPropertyContext(IPropertyContext* context) override;

  [[nodiscard]] auto GetPropertyContext() -> IPropertyContext* override;

  [[nodiscard]] auto GetContextName() const -> std::string_view override;

  [[nodiscard]] auto CanUndo() const -> bool override;

  [[nodiscard]] auto CanRedo() const -> bool override;

  [[nodiscard]] auto IsClean() const -> bool override;

  [[nodiscard]] auto HasPath() const -> bool override;

  [[nodiscard]] auto GetUndoText() const -> std::string override;

  [[nodiscard]] auto GetRedoText() const -> std::string override;

  [[nodiscard]] auto GetPath() const -> std::filesystem::path override;

  [[nodiscard]] auto GetAbsolutePath() const -> std::filesystem::path override;

  /// \} End of document API

  /// \name Map document API
  /// \{

  void AddStampSequence(TileCache&& oldState, TileCache&& newState);

  void AddEraserSequence(TileCache&& oldState);

  /// \command
  void Flood(MapPosition origin, tile_id replacement);

  /// \command
  void AddRow();

  /// \command
  void AddColumn();

  /// \command
  void RemoveRow();

  /// \command
  void RemoveColumn();

  // This function doesn't affect the command stack
  void AddLayer(layer_id id, SharedLayer layer);

  /// \command
  auto AddTileLayer() -> layer_id;

  /// \command
  auto AddObjectLayer() -> layer_id;

  /// \command
  void SelectLayer(layer_id id);

  /// \command
  void RemoveLayer(layer_id id);

  /// \command
  void DuplicateLayer(layer_id id);

  /// \command
  void MoveLayerUp(layer_id id);

  /// \command
  void MoveLayerDown(layer_id id);

  /// \command
  void SetLayerOpacity(layer_id id, float opacity);

  /// \command
  void SetLayerVisible(layer_id id, bool visible);

  /// \command
  void ShowProperties();

  /// \command
  void ShowLayerProperties(layer_id id);

  [[nodiscard]] auto CanMoveActiveLayerDown() const -> bool;

  [[nodiscard]] auto CanMoveActiveLayerUp() const -> bool;

  [[nodiscard]] auto CanMoveLayerDown(layer_id id) const -> bool;

  [[nodiscard]] auto CanMoveLayerUp(layer_id id) const -> bool;

  [[nodiscard]] auto IsLayerVisible(layer_id id) const -> bool;

  [[nodiscard]] auto GetMap() -> Map&;

  [[nodiscard]] auto GetMap() const -> const Map&;

  [[nodiscard]] auto GetRowCount() const -> row_t;

  [[nodiscard]] auto GetColumnCount() const -> col_t;

  /// \command
  void AddTileset(const TextureInfo& info, int tileWidth, int tileHeight);

  void SelectTileset(tileset_id id);

  /// \command
  void RemoveTileset(tileset_id id);

  [[nodiscard]] auto GetTilesets() -> TilesetManager&;

  [[nodiscard]] auto GetTilesets() const -> const TilesetManager&;

  void OffsetViewport(float dx, float dy);

  void IncreaseViewportTileSize();

  void DecreaseViewportTileSize();

  void ResetViewportTileSize();

  void SetNextLayerId(layer_id id);

  void SetNextObjectId(object_id id);

  [[nodiscard]] auto CanDecreaseViewportTileSize() const -> bool;

  [[nodiscard]] auto GetViewportInfo() const noexcept -> const ViewportInfo&
  {
    return mViewportInfo;
  }

  /// \} End of map document API

  /// \name Property API
  /// \{

  /// \command
  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  /// \command
  void RemoveProperty(std::string_view name) override;

  /// \command
  void RenameProperty(std::string_view oldName, const std::string& newName) override;

  /// \command
  void SetProperty(std::string_view name, const Property& property) override;

  /// \command
  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

 private:
  Unique<Map> mMap;                    ///< The associated map.
  Unique<TilesetManager> mTilesets;    ///< The associated tilesets.
  Unique<DocumentDelegate> mDelegate;  ///< Delegate for document API.
  int mTileLayerSuffix{1};             ///< Incrementing tile layer suffix.
  int mObjectLayerSuffix{1};           ///< Incrementing object layer suffix.
  ViewportInfo mViewportInfo;

  [[nodiscard]] auto GetViewportOffsetDelta() const -> std::pair<float, float>;
};

}  // namespace Tactile
