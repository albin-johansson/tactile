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
#include "set_tileset_name.hpp"
#include "stamp_sequence.hpp"
#include "tile_layer.hpp"
#include "update_property.hpp"

namespace tactile::core {

MapDocument::MapDocument(QObject* parent)
    : document{parent}
    , mMap{std::make_unique<Map>()}
    , mTilesets{std::make_unique<tileset_manager>()}
    , mDelegate{std::make_unique<document_delegate>()}
{
  SetUp();
}

MapDocument::MapDocument(const row_t nRows, const col_t nCols, QObject* parent)
    : document{parent}
    , mMap{std::make_unique<Map>(nRows, nCols)}
    , mTilesets{std::make_unique<tileset_manager>()}
    , mDelegate{std::make_unique<document_delegate>()}
{
  SetUp();
}

void MapDocument::SetUp()
{
  auto* commands = mDelegate->history();

  // clang-format off
  connect(commands, &CommandStack::cleanChanged, this, &MapDocument::S_CleanChanged);
  connect(commands, &CommandStack::canUndoChanged, this, &MapDocument::S_UndoStateUpdated);
  connect(commands, &CommandStack::canRedoChanged, this, &MapDocument::S_RedoStateUpdated);
  connect(commands, &CommandStack::undoTextChanged, this, &MapDocument::S_UndoTextUpdated);
  connect(commands, &CommandStack::redoTextChanged, this, &MapDocument::S_RedoTextUpdated);

  connect(mDelegate.get(), &document_delegate::added_property,
          this, &MapDocument::S_AddedProperty);

  connect(mDelegate.get(), &document_delegate::about_to_remove_property,
          this, &MapDocument::S_AboutToRemoveProperty);

  connect(mDelegate.get(), &document_delegate::updated_property,
          this, &MapDocument::S_UpdatedProperty);

  connect(mDelegate.get(), &document_delegate::renamed_property,
          this, &MapDocument::S_RenamedProperty);

  connect(mDelegate.get(), &document_delegate::changed_property_type,
          this, &MapDocument::S_ChangedPropertyType);
  // clang-format on
}

void MapDocument::undo()
{
  mDelegate->undo();

  /* Emit clean_changed once more, because we need to take into account that the
     document might not feature an associated file path yet (that is what
     is_clean does) */
  emit S_CleanChanged(is_clean());
}

void MapDocument::redo()
{
  mDelegate->redo();
}

void MapDocument::mark_as_clean()
{
  mDelegate->mark_as_clean();
}

void MapDocument::reset_history()
{
  mDelegate->reset_history();
}

void MapDocument::set_path(QFileInfo path)
{
  mDelegate->set_path(path);
}

auto MapDocument::can_undo() const -> bool
{
  return mDelegate->can_undo();
}

auto MapDocument::can_redo() const -> bool
{
  return mDelegate->can_redo();
}

auto MapDocument::is_clean() const -> bool
{
  return mDelegate->is_clean();
}

auto MapDocument::has_path() const -> bool
{
  return mDelegate->has_path();
}

auto MapDocument::get_undo_text() const -> QString
{
  return mDelegate->get_undo_text();
}

auto MapDocument::get_redo_text() const -> QString
{
  return mDelegate->get_redo_text();
}

auto MapDocument::path() const -> const QFileInfo&
{
  return mDelegate->path();
}

auto MapDocument::absolute_path() const -> QString
{
  return mDelegate->absolute_path();
}

void MapDocument::add_property(const QString& name,
                               const core::property_type type)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->execute<cmd::AddProperty>(mDelegate.get(), name, type);
}

void MapDocument::add_property(const QString& name,
                               const core::property& property)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->execute<cmd::AddProperty>(mDelegate.get(), name, property);
}

void MapDocument::remove_property(const QString& name)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->execute<cmd::RemoveProperty>(mDelegate.get(), name);
}

void MapDocument::rename_property(const QString& oldName,
                                  const QString& newName)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->execute<cmd::RenameProperty>(mDelegate.get(), oldName, newName);
}

void MapDocument::set_property(const QString& name,
                               const core::property& property)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->execute<cmd::UpdateProperty>(mDelegate.get(), name, property);
}

void MapDocument::change_property_type(const QString& name,
                                       const core::property_type type)
{
  const QSignalBlocker blocker{mDelegate.get()};
  mDelegate->execute<cmd::ChangePropertyType>(mDelegate.get(), name, type);
}

auto MapDocument::get_property(const QString& name) const
    -> const core::property&
{
  return mDelegate->get_property(name);
}

auto MapDocument::get_property(const QString& name) -> core::property&
{
  return mDelegate->get_property(name);
}

auto MapDocument::has_property(const QString& name) const -> bool
{
  return mDelegate->has_property(name);
}

auto MapDocument::property_count() const noexcept -> int
{
  return mDelegate->property_count();
}

auto MapDocument::properties() const -> const property_map&
{
  return mDelegate->properties();
}

void MapDocument::Flood(const position& position, const tile_id replacement)
{
  mDelegate->execute<cmd::BucketFill>(this, position, replacement);
}

void MapDocument::AddStampSequence(vector_map<position, tile_id>&& oldState,
                                   vector_map<position, tile_id>&& sequence)
{
  mDelegate->execute<cmd::StampSequence>(this,
                                         std::move(oldState),
                                         std::move(sequence));
}

void MapDocument::AddEraseSequence(vector_map<position, tile_id>&& oldState)
{
  mDelegate->execute<cmd::EraseSequence>(this, std::move(oldState));
}

void MapDocument::AddRow()
{
  mDelegate->execute<cmd::AddRow>(this);
}

void MapDocument::AddColumn()
{
  mDelegate->execute<cmd::AddColumn>(this);
}

void MapDocument::RemoveRow()
{
  mDelegate->execute<cmd::RemoveRow>(this);
}

void MapDocument::RemoveColumn()
{
  mDelegate->execute<cmd::RemoveColumn>(this);
}

void MapDocument::Resize(const row_t nRows, const col_t nCols)
{
  Q_ASSERT(nRows > 0_row);
  Q_ASSERT(nCols > 0_col);
  mDelegate->execute<cmd::ResizeMap>(this, nRows, nCols);
}

void MapDocument::AddTileset(const QImage& image,
                             const QFileInfo& path,
                             const QString& name,
                             const tile_width tileWidth,
                             const tile_height tileHeight)
{
  if (!image.isNull())
  {
    const auto id = mTilesets->next_tileset_id();
    const auto gid = mTilesets->next_global_tile_id();

    auto ts = std::make_shared<tileset>(gid, image, tileWidth, tileHeight);
    ts->set_name(name);
    ts->set_path(path);

    // This will cause an `added_tileset` signal to be emitted
    mDelegate->execute<cmd::AddTileset>(this, std::move(ts), id);
    mTilesets->increment_next_tileset_id();
  }
}

void MapDocument::RemoveTileset(const tileset_id id)
{
  mDelegate->execute<cmd::RemoveTileset>(this,
                                         mTilesets->get_tileset_pointer(id),
                                         id);
}

void MapDocument::SelectTileset(const tileset_id id)
{
  mTilesets->select(id);
}

void MapDocument::SetTilesetSelection(const tileset_selection& selection)
{
  mTilesets->set_selection(selection);
}

void MapDocument::SelectLayer(const layer_id id)
{
  mDelegate->execute<cmd::SelectLayer>(this, id);
}

void MapDocument::AddLayer(const layer_id id, const shared<ILayer>& layer)
{
  Q_ASSERT(layer);
  mMap->AddLayer(id, layer);
}

auto MapDocument::AddTileLayer() -> layer_id
{
  const auto id = mMap->NextLayerId();  // must be before make_tile_layer
  auto layer = mMap->MakeTileLayer();

  layer->SetName(layer->Name() + TACTILE_QSTRING(u" ") +
                 QString::number(mTileLayerSuffix));
  ++mTileLayerSuffix;

  mDelegate->execute<cmd::AddLayer>(this, std::move(layer), id);
  return id;
}

auto MapDocument::AddObjectLayer() -> layer_id
{
  const auto id = mMap->NextLayerId();  // must be before make_object_layer
  auto layer = mMap->MakeObjectLayer();

  layer->SetName(layer->Name() + TACTILE_QSTRING(u" ") +
                 QString::number(mObjectLayerSuffix));
  ++mObjectLayerSuffix;

  mDelegate->execute<cmd::AddLayer>(this, std::move(layer), id);
  return id;
}

void MapDocument::RemoveLayer(const layer_id id)
{
  mDelegate->execute<cmd::RemoveLayer>(this, id);
}

auto MapDocument::TakeLayer(const layer_id id) -> shared<ILayer>
{
  return mMap->TakeLayer(id);
}

void MapDocument::DuplicateLayer(const layer_id id)
{
  mDelegate->execute<cmd::DuplicateLayer>(this, id);
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
  mDelegate->execute<cmd::SetLayerVisibility>(this, id, visible);
}

void MapDocument::SetLayerOpacity(const layer_id id, const double opacity)
{
  mDelegate->execute<cmd::ChangeOpacity>(this, id, opacity);
}

void MapDocument::SetLayerName(const layer_id id, const QString& name)
{
  mDelegate->execute<cmd::SetLayerName>(this, id, name);
}

void MapDocument::MoveLayerBack(const layer_id id)
{
  mDelegate->execute<cmd::MoveLayerBack>(this, id);
}

void MapDocument::MoveLayerForward(const layer_id id)
{
  mDelegate->execute<cmd::MoveLayerForward>(this, id);
}

void MapDocument::SetTilesetName(const tileset_id id, const QString& name)
{
  mDelegate->execute<cmd::SetTilesetName>(this, id, name);
}

void MapDocument::SetNextLayerId(const layer_id id) noexcept
{
  mMap->SetNextLayerId(id);
}

void MapDocument::SetNextObjectId(const object_id id) noexcept
{
  mMap->SetNextObjectId(id);
}

auto MapDocument::InBounds(const position& pos) const -> bool
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

auto MapDocument::CurrentTileset() const -> const tileset*
{
  return mTilesets->current_tileset();
}

auto MapDocument::Data() const noexcept -> const Map*
{
  return mMap.get();
}

auto MapDocument::GetTilesets() const noexcept -> const tileset_manager*
{
  return mTilesets.get();
}

auto MapDocument::GetTilesets() noexcept -> tileset_manager*
{
  return mTilesets.get();
}

auto MapDocument::Raw() -> Map&
{
  return *mMap;
}

auto MapDocument::CurrentLayerId() const noexcept -> maybe<layer_id>
{
  return mMap->ActiveLayerId();
}

auto MapDocument::HasLayer(const layer_id id) const -> bool
{
  return mMap->HasLayer(id);
}

}  // namespace tactile::core
