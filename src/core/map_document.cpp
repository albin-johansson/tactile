#include "map_document.hpp"

#include <utility>  // move

namespace tactile {

MapDocument::MapDocument()
    : mMap{std::make_unique<Map>()}
    , mTilesets{std::make_unique<TilesetManager>()}
    , mDelegate{std::make_unique<DocumentDelegate>()}
{}

MapDocument::MapDocument(const row_t nRows, const col_t nCols)
    : mMap{std::make_unique<Map>(nRows, nCols)}
    , mTilesets{std::make_unique<TilesetManager>()}
    , mDelegate{std::make_unique<DocumentDelegate>()}
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

}  // namespace tactile
