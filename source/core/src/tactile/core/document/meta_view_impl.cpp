// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/meta_view_impl.hpp"

#include <iterator>  // next

#include "tactile/base/document/document.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

MetaViewImpl::MetaViewImpl(const IDocument* document, const EntityID meta_id) :
  mDocument {require_not_null(document, "null document")},
  mMetaId {meta_id}
{}

auto MetaViewImpl::get_name() const -> StringView
{
  const auto& registry = mDocument->get_registry();
  const auto& meta = registry.get<CMeta>(mMetaId);

  return meta.name;
}

auto MetaViewImpl::get_property(const usize index) const
    -> Pair<const String&, const Attribute&>
{
  const auto& registry = mDocument->get_registry();
  const auto& meta = registry.get<CMeta>(mMetaId);

  if (index >= meta.properties.size()) {
    throw Exception {"bad property index"};
  }

  const auto iter = std::next(meta.properties.begin(), saturate_cast<ssize>(index));
  return {iter->first, iter->second};
}

auto MetaViewImpl::property_count() const -> usize
{
  const auto& registry = mDocument->get_registry();
  const auto& meta = registry.get<CMeta>(mMetaId);

  return meta.properties.size();
}

}  // namespace tactile
