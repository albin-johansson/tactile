#include "model.hpp"

#include "layer_model.hpp"
#include "map_document.hpp"
#include "property_model.hpp"

namespace tactile::core {

Model::Model() : mTools{this}
{}

auto Model::AddMap() -> map_id
{
  return AddMap(new MapDocument{5_row, 5_col, this});
}

auto Model::AddMap(MapDocument* document) -> map_id
{
  Q_ASSERT(document);
  Q_ASSERT(!mDocuments.contains(mNextId));

  document->setParent(this);

  const auto bind = [document, this](auto&& signal, auto&& slot) {
    QObject::connect(document, signal, this, slot);
  };

  bind(&MapDocument::S_Redraw, &Model::S_Redraw);
  bind(&MapDocument::S_UndoStateUpdated, &Model::S_UndoStateUpdated);
  bind(&MapDocument::S_RedoStateUpdated, &Model::S_RedoStateUpdated);
  bind(&MapDocument::S_UndoTextUpdated, &Model::S_UndoTextUpdated);
  bind(&MapDocument::S_RedoTextUpdated, &Model::S_RedoTextUpdated);
  bind(&MapDocument::S_CleanChanged, &Model::S_CleanChanged);
  bind(&MapDocument::S_RemovedTileset, &Model::S_RemovedTileset);
  bind(&MapDocument::S_RenamedTileset, &Model::S_RenamedTileset);
  bind(&MapDocument::S_AddedLayer, &Model::S_AddedLayer);
  bind(&MapDocument::S_AddedDuplicatedLayer, &Model::S_AddedDuplicatedLayer);
  bind(&MapDocument::S_RemovedLayer, &Model::S_RemovedLayer);
  bind(&MapDocument::S_SelectedLayer, &Model::S_SelectedLayer);
  bind(&MapDocument::S_MovedLayerBack, &Model::S_MovedLayerBack);
  bind(&MapDocument::S_MovedLayerForward, &Model::S_MovedLayerForward);
  bind(&MapDocument::S_AddedProperty, &Model::S_AddedProperty);
  bind(&MapDocument::S_AboutToRemoveProperty, &Model::S_AboutToRemoveProperty);
  bind(&MapDocument::S_UpdatedProperty, &Model::S_UpdatedProperty);
  bind(&MapDocument::S_RenamedProperty, &Model::S_RenamedProperty);
  bind(&MapDocument::S_ChangedPropertyType, &Model::S_ChangedPropertyType);

  bind(&MapDocument::S_AddedTileset, [this](const tileset_id id) {
    const auto& tileset = CurrentDocument()->GetTilesets()->At(id);
    emit S_AddedTileset(CurrentMapId().value(), id, tileset);
  });

  bind(&MapDocument::S_ShowProperties,
       [this] { OnShowMapProperties(CurrentMapId().value()); });
  bind(&MapDocument::S_ShowLayerProperties, &Model::OnShowLayerProperties);

  const auto id = mNextId;
  mDocuments.emplace(id, document);
  mCurrentMap = id;

  ++mNextId;

  EmitUndoRedoUpdate();

  return id;
}

auto Model::HasActiveMap() const noexcept -> bool
{
  return mCurrentMap.has_value();
}

auto Model::GetDocument(const map_id id) -> MapDocument*
{
  return mDocuments.at(id);
}

auto Model::CurrentMapId() const -> Maybe<map_id>
{
  return mCurrentMap;
}

auto Model::CurrentDocument() -> MapDocument*
{
  return mCurrentMap ? mDocuments.at(mCurrentMap.value()) : nullptr;
}

auto Model::CurrentDocument() const -> const MapDocument*
{
  return mCurrentMap ? mDocuments.at(mCurrentMap.value()) : nullptr;
}

void Model::Undo()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->Undo();
}

void Model::Redo()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->Redo();
}

void Model::ResizeMap(const row_t nRows, const col_t nCols)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->Resize(nRows, nCols);
}

void Model::AddRow()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->AddRow();
}

void Model::AddColumn()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->AddColumn();
}

void Model::RemoveRow()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->RemoveRow();
}

void Model::RemoveColumn()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->RemoveColumn();
}

void Model::AddLayer()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->AddTileLayer();
}

void Model::RemoveLayer(const layer_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->RemoveLayer(id);
}

void Model::SelectLayer(const layer_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->SelectLayer(id);
}

void Model::SelectTool(const MouseToolType id)
{
  mTools.Select(id);
}

void Model::SelectTileset(const tileset_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->SelectTileset(id);
}

void Model::SetTilesetSelection(const TilesetSelection& selection)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->SetTilesetSelection(selection);
}

void Model::IncreaseTileSize()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->IncreaseTileSize();
}

void Model::DecreaseTileSize()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->DecreaseTileSize();
}

void Model::ResetTileSize()
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->ResetTileSize();
}

void Model::CreateTileset(const QImage& image,
                          const QFileInfo& path,
                          const QString& name,
                          const tile_width tileWidth,
                          const tile_height tileHeight)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->AddTileset(image, path, name, tileWidth, tileHeight);
}

void Model::RemoveTileset(const tileset_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->RemoveTileset(id);
}

void Model::SetTilesetName(const tileset_id id, const QString& name)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->SetTilesetName(id, name);
}

void Model::SelectMap(const map_id id)
{
  Q_ASSERT(mDocuments.contains(id));

  if (mCurrentMap != id)
  {
    mCurrentMap = id;
    EmitUndoRedoUpdate();
    emit S_CleanChanged(CurrentDocument()->IsClean());
  }

  auto* document = CurrentDocument();
  Q_ASSERT(document);

  emit S_SwitchedMap(id, document);
}

void Model::CloseMap(const map_id id)
{
  Q_ASSERT(mDocuments.contains(id));

  mDocuments.at(id)->disconnect();
  mDocuments.erase(id);

  if (mCurrentMap && (mCurrentMap->get() == id.get()))
  {
    mCurrentMap = nothing;
    EmitUndoRedoUpdate();
  }

  if (!mDocuments.empty())
  {
    mCurrentMap = mDocuments.begin()->first;
    EmitUndoRedoUpdate();
  }
}

void Model::OnMousePressed(QMouseEvent* event, const QPointF& mapPosition)
{
  mTools.OnMousePressed(event, mapPosition);
}

void Model::OnMouseMoved(QMouseEvent* event, const QPointF& mapPosition)
{
  mTools.OnMouseMoved(event, mapPosition);
}

void Model::OnMouseReleased(QMouseEvent* event, const QPointF& mapPosition)
{
  mTools.OnMouseReleased(event, mapPosition);
}

void Model::OnMouseEntered(QEvent* event)
{
  mTools.OnMouseEntered(event);
}

void Model::OnMouseExited(QEvent* event)
{
  mTools.OnMouseExited(event);
}

void Model::OnShowMapProperties(const map_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  document->ResetPropertyContext();
  emit S_ShowMapProperties(document);
}

void Model::OnShowLayerProperties(const layer_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  auto* layer = document->GetLayer(id);
  Q_ASSERT(layer);

  document->SetPropertyContext(layer);
  emit S_ShowLayerProperties(document);
}

void Model::OnShowTilesetProperties(const tileset_id id)
{
  auto* document = CurrentDocument();
  Q_ASSERT(document);

  auto* tileset = document->GetTileset(id);
  Q_ASSERT(tileset);

  document->SetPropertyContext(tileset);
  emit S_ShowTilesetProperties(document);
}

void Model::EmitUndoRedoUpdate()
{
  if (const auto* document = CurrentDocument())
  {
    emit S_UndoStateUpdated(document->CanUndo());
    emit S_RedoStateUpdated(document->CanRedo());
    emit S_UndoTextUpdated(document->GetUndoText());
    emit S_RedoTextUpdated(document->GetRedoText());
  }
  else
  {
    emit S_UndoStateUpdated(false);
    emit S_RedoStateUpdated(false);
    emit S_UndoTextUpdated(QString{});
    emit S_RedoTextUpdated(QString{});
  }
}

}  // namespace tactile::core
