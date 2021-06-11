#pragma once

#include <QFileInfo>  // QFileInfo
#include <QImage>     // QImage
#include <QObject>    // QObject

#include "layer_id.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "mouse_tool_model.hpp"
#include "mouse_tool_type.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"
#include "tile_height.hpp"
#include "tile_width.hpp"
#include "tileset_id.hpp"
#include "tileset_selection.hpp"
#include "vector_map.hpp"

namespace tactile::vm {
class PropertyModel;
class LayerModel;
}  // namespace tactile::vm

namespace tactile::core {

class IPropertyManager;
class MapDocument;
class ILayer;
class Tileset;

/**
 * \class Model
 *
 * \brief Represents the main interface for the core of the application.
 *
 * \since 0.1.0
 *
 * \headerfile model.hpp
 */
class Model final : public QObject
{
  Q_OBJECT

 public:
  Model();

  [[nodiscard]] auto AddMap() -> map_id;

  [[nodiscard]] auto AddMap(MapDocument* document) -> map_id;

  [[nodiscard]] auto HasActiveMap() const noexcept -> bool;

  [[nodiscard]] auto GetDocument(map_id id) -> MapDocument*;

  [[nodiscard]] auto CurrentMapId() const -> maybe<map_id>;

  [[nodiscard]] auto CurrentDocument() -> MapDocument*;

  [[nodiscard]] auto CurrentDocument() const -> const MapDocument*;

 signals:
  void S_Redraw();
  void S_SwitchedMap(map_id id, not_null<MapDocument*> document);

  void S_AddedLayer(layer_id id, const ILayer& layer);
  void S_AddedDuplicatedLayer(layer_id id, const ILayer& layer);
  void S_SelectedLayer(layer_id id, const ILayer& layer);
  void S_RemovedLayer(layer_id id);
  void S_MovedLayerBack(layer_id id);
  void S_MovedLayerForward(layer_id id);

  void S_DisableStampPreview();
  void S_EnableStampPreview(const Position& position);

  void S_UndoStateUpdated(bool canUndo);
  void S_RedoStateUpdated(bool canRedo);
  void S_UndoTextUpdated(const QString& text);
  void S_RedoTextUpdated(const QString& text);
  void S_CleanChanged(bool clean);

  void S_AddedTileset(map_id map, tileset_id id, const Tileset& tileset);
  void S_RemovedTileset(tileset_id id);
  void S_RenamedTileset(tileset_id id, const QString& name);

  void S_AddedProperty(const QString& name);
  void S_AboutToRemoveProperty(const QString& name);
  void S_UpdatedProperty(const QString& name);
  void S_ChangedPropertyType(const QString& name);
  void S_RenamedProperty(const QString& oldName, const QString& newName);

  void S_ShowMapProperties(core::IPropertyManager* manager);
  void S_ShowLayerProperties(core::IPropertyManager* manager);

 public slots:
  /**
   * \brief Reverts the effects of the last performed command.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \since 0.1.0
   *
   * \signal `S_Redraw`, `S_CleanChanged`
   */
  void Undo();

  /**
   * \brief Restores the effects of the last undone command.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \since 0.1.0
   *
   * \signal `S_Redraw`
   */
  void Redo();

  /**
   * \brief Resizes the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \param nRows the new number of rows, must be greater than 0.
   * \param nCols the new number of columns, must be greater than 0.
   *
   * \see cmd::ResizeMap
   *
   * \since 0.1.0
   *
   * \signal `S_Redraw`
   */
  void ResizeMap(row_t nRows, col_t nCols);

  /**
   * \brief Adds a row to the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::AddRow
   *
   * \since 0.1.0
   *
   * \signal `S_Redraw`
   */
  void AddRow();

  /**
   * \brief Adds a column to the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::AddColumn
   *
   * \since 0.1.0
   *
   * \signal `S_Redraw`
   */
  void AddColumn();

  /**
   * \brief Removes a row from the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::RemoveRow
   *
   * \since 0.1.0
   */
  void RemoveRow();

  /**
   * \brief Removes a column from the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::RemoveColumn
   *
   * \since 0.1.0
   */
  void RemoveColumn();

  /**
   * \brief Adds a layer to the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \since 0.1.0
   *
   * \signal `S_AddedLayer`
   */
  void AddLayer();

  /**
   * \brief Removes the specified layer from the currently active map.
   *
   * \param id the ID associated with the layer that will be removed.
   *
   * \since 0.1.0
   *
   * \signal `S_RemovedLayer`.
   */
  void RemoveLayer(layer_id id);

  /**
   * \brief Selects the tile layer associated with the specified index.
   *
   * \note This method has no effect if the supplied index isn't associated
   * with a tile layer or if there is no active map document.
   *
   * \param id the index of the tile layer that will be selected.
   *
   * \since 0.1.0
   *
   * \signal `S_SelectedLayer`, `S_Redraw`
   */
  void SelectLayer(layer_id id);

  void SelectTool(MouseToolType id);

  void SelectTileset(tileset_id id);

  void SetTilesetSelection(const TilesetSelection& selection);

  /**
   * \brief Increases the tile size that is being used by the currently active
   * map.
   *
   * \since 0.1.0
   */
  void IncreaseTileSize();

  /**
   * \brief Decreases the tile size that is being used by the currently active
   * map.
   *
   * \since 0.1.0
   */
  void DecreaseTileSize();

  /**
   * \brief Resets the tile size that is being used by the currently active
   * map to its default value.
   *
   * \since 0.1.0
   */
  void ResetTileSize();

  void CreateTileset(const QImage& image,
                     const QFileInfo& path,
                     const QString& name,
                     tile_width tileWidth,
                     tile_height tileHeight);

  void RemoveTileset(tileset_id id);

  void SetTilesetName(tileset_id id, const QString& name);

  void SelectMap(map_id id);
  void CloseMap(map_id id);

  void OnMousePressed(QMouseEvent* event, const QPointF& mapPosition);
  void OnMouseMoved(QMouseEvent* event, const QPointF& mapPosition);
  void OnMouseReleased(QMouseEvent* event, const QPointF& mapPosition);
  void OnMouseEntered(QEvent* event);
  void OnMouseExited(QEvent* event);

 private:
  vector_map<map_id, MapDocument*> mDocuments;
  maybe<map_id> mCurrentMap;
  map_id mNextId{1};
  MouseToolModel mTools;

  void EmitUndoRedoUpdate();
};

}  // namespace tactile::core