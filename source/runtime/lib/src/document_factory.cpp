// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/runtime/document_factory.hpp"

#include <utility>  // move

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/document/map_view_impl.hpp"

namespace tactile::runtime {

auto make_map_document(IRenderer& renderer, const ir::Map& ir_map)
    -> std::unique_ptr<IDocument>
{
  if (auto document = core::MapDocument::make(renderer, ir_map)) {
    return std::make_unique<core::MapDocument>(std::move(*document));
  }

  return nullptr;
}

auto make_map_view(const IDocument& document) -> std::unique_ptr<IMapView>
{
  if (const auto* map_document = dynamic_cast<const core::MapDocument*>(&document)) {
    return std::make_unique<core::MapViewImpl>(map_document);
  }

  return nullptr;
}

}  // namespace tactile::runtime
