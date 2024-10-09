// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/entity/registry.hpp"

namespace tactile::core {

auto Registry::make_entity() -> EntityID
{
  return mRegistry.create();
}

auto Registry::destroy(const EntityID entity_id) -> bool
{
  if (is_valid(entity_id)) [[likely]] {
    mRegistry.destroy(entity_id);
    return true;
  }

  return false;
}

auto Registry::count() const -> std::size_t
{
  std::size_t n = 0;

  for (const auto& [id, sparse_set] : mRegistry.storage()) {
    n += sparse_set.size();
  }

  return n;
}

auto Registry::is_valid(const EntityID entity) const -> bool
{
  return mRegistry.valid(entity);
}

}  // namespace tactile::core
