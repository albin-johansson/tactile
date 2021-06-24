#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/map_id.hpp"
#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "map_document.hpp"

namespace tactile {

class Model final
{
 public:
  Model();

  [[nodiscard]] auto AddMap() -> map_id;

  void SelectMap(map_id id);

  [[nodiscard]] auto GetDocument(map_id id) -> MapDocument*;

  [[nodiscard]] auto GetDocument(map_id id) const -> const MapDocument*;

  [[nodiscard]] auto GetActiveDocument() -> MapDocument*;

  [[nodiscard]] auto GetActiveDocument() const -> const MapDocument*;

  [[nodiscard]] auto GetActiveMapId() const -> Maybe<map_id>
  {
    return mActiveMap;
  }

 private:
  rune::vector_map<map_id, Unique<MapDocument>> mDocuments;
  Maybe<map_id> mActiveMap;
  map_id mNextId{1};
};

}  // namespace tactile
