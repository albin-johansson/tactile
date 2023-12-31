// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/map_document.hpp"

#include <utility>  // move

namespace tactile {

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

}  // namespace tactile
