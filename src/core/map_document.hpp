#pragma once

#include "aliases/ints.hpp"
#include "aliases/unique.hpp"
#include "core/document.hpp"
#include "core/document_delegate.hpp"
#include "core/map/map.hpp"
#include "core/properties/property_delegate.hpp"
#include "core/tileset/tileset_manager.hpp"
#include "utils/texture_info.hpp"

namespace Tactile {

class MapDocument final : public IDocument
{
 public:
  MapDocument();

  MapDocument(row_t nRows, col_t nCols);

  /// \name Document API
  /// \{

  void Undo() override;

  void Redo() override;

  void MarkAsClean() override;

  void ResetHistory() override;

  void SetPath(std::filesystem::path path) override;

  void SetPropertyContext(IPropertyContext* context) override;

  [[nodiscard]] auto GetPropertyContext() -> IPropertyContext* override;

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

  void AddRow();

  void AddColumn();

  void RemoveRow();

  void RemoveColumn();

  [[nodiscard]] auto GetMap() -> Map&;

  [[nodiscard]] auto GetMap() const -> const Map&;

  [[nodiscard]] auto GetRowCount() const -> row_t;

  [[nodiscard]] auto GetColumnCount() const -> col_t;

  void AddTileset(const TextureInfo& info, int tileWidth, int tileHeight);

  void SelectTileset(tileset_id id);

  void RemoveTileset(tileset_id id);

  [[nodiscard]] auto GetTilesets() -> TilesetManager&;

  [[nodiscard]] auto GetTilesets() const -> const TilesetManager&;

  /// \} End of map document API

  /// \name Property API
  /// \{

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName,
                      const std::string& newName) override;

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
  Unique<Map> mMap;                    ///< The associated map.
  Unique<TilesetManager> mTilesets;    ///< The associated tilesets.
  Unique<DocumentDelegate> mDelegate;  ///< Delegate for document API.
  int mTileLayerSuffix{1};             ///< Incrementing tile layer suffix.
  int mObjectLayerSuffix{1};           ///< Incrementing object layer suffix.
};

}  // namespace Tactile
