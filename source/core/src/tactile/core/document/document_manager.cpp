// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/document_manager.hpp"

#include "tactile/core/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

auto DocumentManager::get_document(const UUID& uuid) -> IDocument&
{
  return *lookup_in(mDocuments, uuid);
}

auto DocumentManager::get_document(const UUID& uuid) const -> const IDocument&
{
  return *lookup_in(mDocuments, uuid);
}

auto DocumentManager::get_current_document() -> IDocument*
{
  if (!mActiveDocument.is_null()) {
    return &get_document(mActiveDocument);
  }

  return nullptr;
}

auto DocumentManager::get_current_document() const -> const IDocument*
{
  if (!mActiveDocument.is_null()) {
    return &get_document(mActiveDocument);
  }

  return nullptr;
}

auto DocumentManager::get_open_documents() const -> const Vector<UUID>&
{
  return mOpenDocuments;
}

auto DocumentManager::is_map_active() const -> bool
{
  if (!mActiveDocument.is_null()) {
    const auto& document = get_document(mActiveDocument);
    return is_map(document.get_registry(), document.get_root_entity());
  }

  return false;
}

}  // namespace tactile
