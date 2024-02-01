// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/map_document.hpp"

#include <utility>  // move

#include "tactile/core/meta/meta_context_finder.hpp"

namespace tactile::core {

MapDocument::MapDocument(Unique<IMap> map)
  : mMap {std::move(map)}
{}

void MapDocument::update()
{
}

void MapDocument::set_path(FilePath path)
{
  mPath = std::move(path);
}

auto MapDocument::path() const -> const FilePath*
{
  return mPath.has_value() ? &*mPath : nullptr;
}

auto MapDocument::find_context(const UUID& uuid) -> IMetaContext*
{
  MetaContextFinder finder {uuid};
  mMap->accept(finder);
  return finder.found_context();
}

auto MapDocument::find_context(const UUID& uuid) const -> const IMetaContext*
{
  MetaContextFinder finder {uuid};
  mMap->accept(finder);
  return finder.found_context();
}

auto MapDocument::component_set() -> ComponentSet&
{
  return mComponentSet;
}

auto MapDocument::component_set() const -> const ComponentSet&
{
  return mComponentSet;
}

auto MapDocument::viewport() -> DocumentViewport&
{
  return mViewport;
}

auto MapDocument::viewport() const -> const DocumentViewport&
{
  return mViewport;
}

auto MapDocument::map() -> IMap&
{
  return *mMap;
}

auto MapDocument::map() const -> const IMap&
{
  return *mMap;
}

}  // namespace tactile::core
