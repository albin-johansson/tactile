// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/entity/entity.hpp"

#include <utility>  // exchange

namespace tactile::core {

Entity::Entity(Registry* registry, const EntityID id) noexcept
  : m_registry {registry},
    m_id {id}
{}

void Entity::try_destroy() noexcept
{
  if (has_value()) {
    m_registry->destroy(m_id);

    m_registry = nullptr;
    m_id = kInvalidEntity;
  }
}

auto Entity::get_registry() -> Registry&
{
  assert(has_value());
  return *m_registry;
}

auto Entity::get_registry() const -> const Registry&
{
  assert(has_value());
  return *m_registry;
}

auto Entity::get_id() const noexcept -> EntityID
{
  return m_id;
}

auto Entity::has_value() const noexcept -> bool
{
  return m_registry != nullptr && m_id != kInvalidEntity;
}

Entity::operator bool() const noexcept
{
  return has_value();
}

}  // namespace tactile::core
