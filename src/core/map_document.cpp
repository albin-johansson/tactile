#include "map_document.hpp"

#include <cassert>  // assert
#include <format>   // format
#include <utility>  // move

#include "core/commands/maps/add_column_cmd.hpp"
#include "core/commands/maps/add_row_cmd.hpp"
#include "core/commands/maps/layers/add_layer_cmd.hpp"
#include "core/commands/maps/layers/duplicate_layer_cmd.hpp"
#include "core/commands/maps/layers/move_layer_down_cmd.hpp"
#include "core/commands/maps/layers/move_layer_up_cmd.hpp"
#include "core/commands/maps/layers/remove_layer_cmd.hpp"
#include "core/commands/maps/layers/select_layer_cmd.hpp"
#include "core/commands/maps/layers/set_layer_opacity_cmd.hpp"
#include "core/commands/maps/remove_column_cmd.hpp"
#include "core/commands/maps/remove_row_cmd.hpp"
#include "core/commands/set_property_context_cmd.hpp"

namespace Tactile {

MapDocument::MapDocument()
    : mMap{std::make_unique<Map>()}
    , mTilesets{std::make_unique<TilesetManager>()}
    , mDelegate{std::make_unique<DocumentDelegate>("Map")}
{}

MapDocument::MapDocument(const row_t nRows, const col_t nCols)
    : mMap{std::make_unique<Map>(nRows, nCols)}
    , mTilesets{std::make_unique<TilesetManager>()}
    , mDelegate{std::make_unique<DocumentDelegate>("Map")}
{}

void MapDocument::Undo()
{
  mDelegate->Undo();
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

void MapDocument::SetPath(std::filesystem::path path)
{
  mDelegate->SetPath(std::move(path));
}

void MapDocument::SetPropertyContext(IPropertyContext* context)
{
  mDelegate->SetPropertyContext(context);
}

auto MapDocument::GetPropertyContext() -> IPropertyContext*
{
  return mDelegate->GetPropertyContext();
}

auto MapDocument::GetContextName() const -> std::string_view
{
  return mDelegate->GetContextName();
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

auto MapDocument::GetUndoText() const -> std::string
{
  return mDelegate->GetUndoText();
}

auto MapDocument::GetRedoText() const -> std::string
{
  return mDelegate->GetRedoText();
}

auto MapDocument::GetPath() const -> std::filesystem::path
{
  return mDelegate->GetPath();
}

auto MapDocument::GetAbsolutePath() const -> std::filesystem::path
{
  return mDelegate->GetAbsolutePath();
}

void MapDocument::AddRow()
{
  mDelegate->Execute<AddRowCmd>(this);
}

void MapDocument::AddColumn()
{
  mDelegate->Execute<AddColumnCmd>(this);
}

void MapDocument::RemoveRow()
{
  mDelegate->Execute<RemoveRowCmd>(this);
}

void MapDocument::RemoveColumn()
{
  mDelegate->Execute<RemoveColumnCmd>(this);
}

auto MapDocument::AddTileLayer() -> layer_id
{
  const auto id = mMap->GetNextLayerId();
  auto layer = mMap->MakeTileLayer();  // This increments the next layer ID

  layer->SetName(std::format("{} {}", layer->GetName(), mTileLayerSuffix));
  ++mTileLayerSuffix;

  mDelegate->Execute<AddLayerCmd>(this, std::move(layer), id);

  return id;
}

auto MapDocument::AddObjectLayer() -> layer_id
{
  const auto id = mMap->GetNextLayerId();
  auto layer = mMap->MakeObjectLayer();  // This increments the next layer ID

  layer->SetName(std::format("{} {}", layer->GetName(), mObjectLayerSuffix));
  ++mObjectLayerSuffix;

  mDelegate->Execute<AddLayerCmd>(this, std::move(layer), id);

  return id;
}

void MapDocument::SelectLayer(const layer_id id)
{
  mDelegate->Execute<SelectLayerCmd>(this, id);
}

void MapDocument::RemoveLayer(const layer_id id)
{
  mDelegate->Execute<RemoveLayerCmd>(this, id);
}

void MapDocument::DuplicateLayer(const layer_id id)
{
  mDelegate->Execute<DuplicateLayerCmd>(this, id);
}

void MapDocument::MoveLayerUp(const layer_id id)
{
  mDelegate->Execute<MoveLayerUpCmd>(this, id);
}

void MapDocument::MoveLayerDown(const layer_id id)
{
  mDelegate->Execute<MoveLayerDownCmd>(this, id);
}

void MapDocument::SetLayerOpacity(const layer_id id, const float opacity)
{
  mDelegate->Execute<SetLayerOpacityCmd>(this, id, opacity);
}

void MapDocument::ShowProperties()
{
  mDelegate->Execute<SetPropertyContextCmd>(this, nullptr);
}

void MapDocument::ShowLayerProperties(const layer_id id)
{
  auto& layer = mMap->GetLayer(id);
  mDelegate->Execute<SetPropertyContextCmd>(this, layer.get());
}

auto MapDocument::CanMoveActiveLayerDown() const -> bool
{
  const auto id = mMap->GetActiveLayerId();
  return id && CanMoveLayerDown(*id);
}

auto MapDocument::CanMoveActiveLayerUp() const -> bool
{
  const auto id = mMap->GetActiveLayerId();
  return id && CanMoveLayerUp(*id);
}

auto MapDocument::CanMoveLayerDown(const layer_id id) const -> bool
{
  return mMap->IndexOf(id) != (mMap->GetLayerCount() - 1);
}

auto MapDocument::CanMoveLayerUp(const layer_id id) const -> bool
{
  return mMap->IndexOf(id) != 0;
}

auto MapDocument::GetMap() -> Map&
{
  return *mMap;
}

auto MapDocument::GetMap() const -> const Map&
{
  return *mMap;
}

auto MapDocument::GetRowCount() const -> row_t
{
  return mMap->GetRowCount();
}

auto MapDocument::GetColumnCount() const -> col_t
{
  return mMap->GetColumnCount();
}

void MapDocument::AddTileset(const TextureInfo& info,
                             const int tileWidth,
                             const int tileHeight)
{
  const auto tilesetId = mTilesets->GetNextTilesetId();
  const auto tileId = mTilesets->GetNextGlobalTileId();

  auto tileset = std::make_shared<Tileset>(tileId, info, tileWidth, tileHeight);
  mTilesets->Add(tilesetId, std::move(tileset));
  mTilesets->IncrementNextTilesetId();
}

void MapDocument::SelectTileset(const tileset_id id)
{
  mTilesets->Select(id);
}

void MapDocument::RemoveTileset(const tileset_id id)
{
  mTilesets->Remove(id);
}

auto MapDocument::GetTilesets() -> TilesetManager&
{
  assert(mTilesets);
  return *mTilesets;
}

auto MapDocument::GetTilesets() const -> const TilesetManager&
{
  assert(mTilesets);
  return *mTilesets;
}

void MapDocument::AddProperty(const std::string& name, const PropertyType type)
{
  mDelegate->AddProperty(name, type);
}

void MapDocument::AddProperty(const std::string& name, const Property& property)
{
  mDelegate->AddProperty(name, property);
}

void MapDocument::RemoveProperty(const std::string_view name)
{
  mDelegate->RemoveProperty(name);
}

void MapDocument::RenameProperty(const std::string_view oldName,
                                 const std::string& newName)
{
  mDelegate->RenameProperty(oldName, newName);
}

void MapDocument::SetProperty(const std::string_view name,
                              const Property& property)
{
  mDelegate->SetProperty(name, property);
}

void MapDocument::ChangePropertyType(const std::string_view name,
                                     const PropertyType type)
{
  mDelegate->ChangePropertyType(name, type);
}

auto MapDocument::HasProperty(const std::string_view name) const -> bool
{
  return mDelegate->HasProperty(name);
}

auto MapDocument::GetProperty(const std::string_view name) const
    -> const Property&
{
  return mDelegate->GetProperty(name);
}

auto MapDocument::GetProperties() const -> const PropertyMap&
{
  return mDelegate->GetProperties();
}

auto MapDocument::GetPropertyCount() const -> usize
{
  return mDelegate->GetPropertyCount();
}

auto MapDocument::GetName() const -> std::string_view
{
  return mDelegate->GetName();
}

}  // namespace Tactile
