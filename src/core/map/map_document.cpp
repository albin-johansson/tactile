#include "map_document.hpp"

#include <utility>  // move

#include "add_column.hpp"
#include "add_layer.hpp"
#include "add_property.hpp"
#include "add_row.hpp"
#include "add_tileset.hpp"
#include "bucket_fill.hpp"
#include "change_opacity.hpp"
#include "change_property_type.hpp"
#include "document_delegate.hpp"
#include "duplicate_layer.hpp"
#include "erase_sequence.hpp"
#include "move_layer_back.hpp"
#include "move_layer_forward.hpp"
#include "object_layer.hpp"
#include "remove_column.hpp"
#include "remove_layer.hpp"
#include "remove_property.hpp"
#include "remove_row.hpp"
#include "remove_tileset.hpp"
#include "rename_property.hpp"
#include "resize_map.hpp"
#include "select_layer.hpp"
#include "set_layer_name.hpp"
#include "set_layer_visibility.hpp"
#include "set_property_context.hpp"
#include "set_tileset_name.hpp"
#include "stamp_sequence.hpp"
#include "tile_layer.hpp"
#include "update_property.hpp"

namespace tactile::core {

MapDocument::MapDocument(QObject* parent)
    : ADocument{parent}
    , mMap{std::make_unique<Map>()}
    , mTilesets{std::make_unique<TilesetManager>()}
    , mDelegate{std::make_unique<DocumentDelegate>()}
{
  SetUp();
}

MapDocument::MapDocument(const row_t nRows, const col_t nCols, QObject* parent)
    : ADocument{parent}
    , mMap{std::make_unique<Map>(nRows, nCols)}
    , mTilesets{std::make_unique<TilesetManager>()}
    , mDelegate{std::make_unique<DocumentDelegate>()}
{
  SetUp();
}

void MapDocument::SetUp()
{
  setObjectName(QStringLiteral(u"MapDocument"));

  auto* commands = mDelegate->History();

  // clang-format off
  connect(commands, &CommandStack::cleanChanged, this, &MapDocument::S_CleanChanged);
  connect(commands, &CommandStack::canUndoChanged, this, &MapDocument::S_UndoStateUpdated);
  connect(commands, &CommandStack::canRedoChanged, this, &MapDocument::S_RedoStateUpdated);
  connect(commands, &CommandStack::undoTextChanged, this, &MapDocument::S_UndoTextUpdated);
  connect(commands, &CommandStack::redoTextChanged, this, &MapDocument::S_RedoTextUpdated);

  connect(mDelegate.get(), &DocumentDelegate::S_AddedProperty, this, &MapDocument::S_AddedProperty);
  connect(mDelegate.get(), &DocumentDelegate::S_AboutToRemoveProperty, this, &MapDocument::S_AboutToRemoveProperty);
  connect(mDelegate.get(), &DocumentDelegate::S_UpdatedProperty, this, &MapDocument::S_UpdatedProperty);
  connect(mDelegate.get(), &DocumentDelegate::S_RenamedProperty, this, &MapDocument::S_RenamedProperty);
  connect(mDelegate.get(), &DocumentDelegate::S_ChangedPropertyType, this, &MapDocument::S_ChangedPropertyType);
  // clang-format on
}

void MapDocument::Undo()
{
  mDelegate->Undo();

  /* Emit S_CleanChanged once more, because we need to take into account that
     the document might not feature an associated file path yet (that is what
     IsClean does) */
  emit S_CleanChanged(IsClean());
}

void MapDocument::Redo()
{
  mDelegate->Redo();
}

void MapDocument::MarkAsClean()
{
  mDelegate->MarkAsClean();
}

void MapDocument::ResetHistory()
{
  mDelegate->ResetHistory();
}

void MapDocument::SetPath(QFileInfo path)
{
  mDelegate->SetPath(path);
}

void MapDocument::SetPropertyContext(IPropertyManager* context)
{
  mDelegate->Execute<cmd::SetPropertyContext>(this, context);
}

auto MapDocument::GetPropertyContext() -> IPropertyManager*
{
  return mDelegate->GetPropertyContext();
}

auto MapDocument::CanUndo() const -> bool
{
  return mDelegate->CanUndo();
}

auto MapDocument::CanRedo() const -> bool
{
  return mDelegate->CanRedo();
}

auto MapDocument::IsClean() const -> bool
{
  return mDelegate->IsClean();
}

auto MapDocument::HasPath() const -> bool
{
  return mDelegate->HasPath();
}

auto MapDocument::GetUndoText() const -> QString
{
  return mDelegate->GetUndoText();
}

auto MapDocument::GetRedoText() const -> QString
{
  return mDelegate->GetRedoText();
}

auto MapDocument::Path() const -> const QFileInfo&
{
  return mDelegate->Path();
}

auto MapDocument::AbsolutePath() const -> QString
{
  return mDelegate->AbsolutePath();
}

void MapDocument::AddProperty(const QString& name, const PropertyType type)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->Execute<cmd::AddProperty>(mDelegate.get(), name, type);
}

void MapDocument::AddProperty(const QString& name,
                              const core::Property& property)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->Execute<cmd::AddProperty>(mDelegate.get(), name, property);
}

void MapDocument::RemoveProperty(const QString& name)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->Execute<cmd::RemoveProperty>(mDelegate.get(), name);
}

void MapDocument::RenameProperty(const QString& oldName, const QString& newName)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->Execute<cmd::RenameProperty>(mDelegate.get(), oldName, newName);
}

void MapDocument::SetProperty(const QString& name,
                              const core::Property& property)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->Execute<cmd::UpdateProperty>(mDelegate.get(), name, property);
}

void MapDocument::ChangePropertyType(const QString& name,
                                     const PropertyType type)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->Execute<cmd::ChangePropertyType>(mDelegate.get(), name, type);
}

auto MapDocument::GetProperty(const QString& name) const
    -> const core::Property&
{
  return mDelegate->GetProperty(name);
}

auto MapDocument::GetProperty(const QString& name) -> core::Property&
{
  return mDelegate->GetProperty(name);
}

auto MapDocument::HasProperty(const QString& name) const -> bool
{
  return mDelegate->HasProperty(name);
}

auto MapDocument::PropertyCount() const noexcept -> int
{
  return mDelegate->PropertyCount();
}

auto MapDocument::GetProperties() const -> const property_map&
{
  return mDelegate->GetProperties();
}

auto MapDocument::GetName() const -> QString
{
  return mDelegate->GetName();
}

void MapDocument::Flood(const MapPosition& position, const tile_id replacement)
{
  mDelegate->Execute<cmd::BucketFill>(this, position, replacement);
}

void MapDocument::AddStampSequence(vector_map<MapPosition, tile_id>&& oldState,
                                   vector_map<MapPosition, tile_id>&& sequence)
{
  mDelegate->Execute<cmd::StampSequence>(this,
                                         std::move(oldState),
                                         std::move(sequence));
}

void MapDocument::AddEraseSequence(vector_map<MapPosition, tile_id>&& oldState)
{
  mDelegate->Execute<cmd::EraseSequence>(this, std::move(oldState));
}

void MapDocument::AddRow()
{
  mDelegate->Execute<cmd::AddRow>(this);
}

void MapDocument::AddColumn()
{
  mDelegate->Execute<cmd::AddColumn>(this);
}

void MapDocument::RemoveRow()
{
  mDelegate->Execute<cmd::RemoveRow>(this);
}

void MapDocument::RemoveColumn()
{
  mDelegate->Execute<cmd::RemoveColumn>(this);
}

void MapDocument::Resize(const row_t nRows, const col_t nCols)
{
  Q_ASSERT(nRows > 0_row);
  Q_ASSERT(nCols > 0_col);
  mDelegate->Execute<cmd::ResizeMap>(this, nRows, nCols);
}

void MapDocument::AddTileset(const QImage& image,
                             const QFileInfo& path,
                             const QString& name,
                             const tile_width tileWidth,
                             const tile_height tileHeight)
{
  if (!image.isNull())
  {
    const auto id = mTilesets->NextTilesetId();
    const auto gid = mTilesets->NextGlobalTileId();

    auto ts = std::make_shared<Tileset>(gid, image, tileWidth, tileHeight);
    ts->SetName(name);
    ts->SetPath(path);

    // This will cause an `added_tileset` signal to be emitted
    mDelegate->Execute<cmd::AddTileset>(this, std::move(ts), id);
    mTilesets->IncrementNextTilesetId();
  }
}

void MapDocument::RemoveTileset(const tileset_id id)
{
  mDelegate->Execute<cmd::RemoveTileset>(this,
                                         mTilesets->GetTilesetPointer(id),
                                         id);
}

void MapDocument::SelectTileset(const tileset_id id)
{
  mTilesets->Select(id);
}

void MapDocument::SetTilesetSelection(const TilesetSelection& selection)
{
  mTilesets->SetSelection(selection);
}

void MapDocument::SelectLayer(const layer_id id)
{
  mDelegate->Execute<cmd::SelectLayer>(this, id);
}

void MapDocument::AddLayer(const layer_id id, const Shared<ILayer>& layer)
{
  Q_ASSERT(layer);
  mMap->AddLayer(id, layer);
}

auto MapDocument::AddTileLayer() -> layer_id
{
  const auto id = mMap->NextLayerId();  // must be before make_tile_layer
  auto layer = mMap->MakeTileLayer();

  layer->SetName(layer->GetName() + QStringLiteral(u" ") +
                 QString::number(mTileLayerSuffix));
  ++mTileLayerSuffix;

  mDelegate->Execute<cmd::AddLayer>(this, std::move(layer), id);
  return id;
}

auto MapDocument::AddObjectLayer() -> layer_id
{
  const auto id = mMap->NextLayerId();  // must be before make_object_layer
  auto layer = mMap->MakeObjectLayer();

  layer->SetName(layer->GetName() + QStringLiteral(u" ") +
                 QString::number(mObjectLayerSuffix));
  ++mObjectLayerSuffix;

  mDelegate->Execute<cmd::AddLayer>(this, std::move(layer), id);
  return id;
}

void MapDocument::RemoveLayer(const layer_id id)
{
  mDelegate->Execute<cmd::RemoveLayer>(this, id);
}

auto MapDocument::TakeLayer(const layer_id id) -> Shared<ILayer>
{
  return mMap->TakeLayer(id);
}

void MapDocument::DuplicateLayer(const layer_id id)
{
  mDelegate->Execute<cmd::DuplicateLayer>(this, id);
}

void MapDocument::IncreaseTileSize()
{
  mMap->IncreaseTileSize();
  emit S_Redraw();
}

void MapDocument::DecreaseTileSize()
{
  mMap->DecreaseTileSize();
  emit S_Redraw();
}

void MapDocument::ResetTileSize()
{
  mMap->ResetTileSize();
  emit S_Redraw();
}

void MapDocument::SetLayerVisibility(const layer_id id, const bool visible)
{
  mDelegate->Execute<cmd::SetLayerVisibility>(this, id, visible);
}

void MapDocument::SetLayerOpacity(const layer_id id, const double opacity)
{
  mDelegate->Execute<cmd::ChangeOpacity>(this, id, opacity);
}

void MapDocument::SetLayerName(const layer_id id, const QString& name)
{
  mDelegate->Execute<cmd::SetLayerName>(this, id, name);
}

void MapDocument::MoveLayerBack(const layer_id id)
{
  mDelegate->Execute<cmd::MoveLayerBack>(this, id);
}

void MapDocument::MoveLayerForward(const layer_id id)
{
  mDelegate->Execute<cmd::MoveLayerForward>(this, id);
}

void MapDocument::SetTilesetName(const tileset_id id, const QString& name)
{
  mDelegate->Execute<cmd::SetTilesetName>(this, id, name);
}

void MapDocument::SetNextLayerId(const layer_id id) noexcept
{
  mMap->SetNextLayerId(id);
}

void MapDocument::SetNextObjectId(const object_id id) noexcept
{
  mMap->SetNextObjectId(id);
}

auto MapDocument::InBounds(const MapPosition& pos) const -> bool
{
  return mMap->InBounds(pos);
}

auto MapDocument::GetLayer(const layer_id id) -> ILayer*
{
  return mMap->GetLayer(id).get();
}

auto MapDocument::GetLayer(const layer_id id) const -> const ILayer*
{
  return mMap->GetLayer(id).get();
}

auto MapDocument::GetTileLayer(const layer_id id) -> TileLayer*
{
  return mMap->GetTileLayer(id);
}

auto MapDocument::GetTileLayer(const layer_id id) const -> const TileLayer*
{
  return mMap->GetTileLayer(id);
}

auto MapDocument::GetObjectLayer(const layer_id id) const -> const ObjectLayer*
{
  return mMap->GetObjectLayer(id);
}

auto MapDocument::LayerCount() const noexcept -> int
{
  return mMap->LayerCount();
}

auto MapDocument::RowCount() const -> row_t
{
  return mMap->RowCount();
}

auto MapDocument::ColumnCount() const -> col_t
{
  return mMap->ColumnCount();
}

auto MapDocument::Width() const -> int
{
  return mMap->Width();
}

auto MapDocument::Height() const -> int
{
  return mMap->Height();
}

auto MapDocument::CurrentTileSize() const noexcept -> int
{
  return mMap->CurrentTileSize();
}

auto MapDocument::CurrentTileset() const -> const Tileset*
{
  return mTilesets->CurrentTileset();
}

auto MapDocument::Data() const noexcept -> const Map*
{
  return mMap.get();
}

auto MapDocument::GetDelegate() -> DocumentDelegate&
{
  return *mDelegate;
}

auto MapDocument::GetTilesets() const noexcept -> const TilesetManager*
{
  return mTilesets.get();
}

auto MapDocument::GetTilesets() noexcept -> TilesetManager*
{
  return mTilesets.get();
}

auto MapDocument::GetTileset(const tileset_id id) -> Tileset*
{
  return mTilesets->GetTilesetPointer(id).get();
}

auto MapDocument::Raw() -> Map&
{
  return *mMap;
}

auto MapDocument::CurrentLayerId() const noexcept -> Maybe<layer_id>
{
  return mMap->ActiveLayerId();
}

auto MapDocument::HasLayer(const layer_id id) const -> bool
{
  return mMap->HasLayer(id);
}

}  // namespace tactile::core
