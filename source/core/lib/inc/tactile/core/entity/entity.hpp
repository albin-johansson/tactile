// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity_id.hpp"
#include "tactile/core/entity/registry.hpp"

namespace tactile::core {

/// A non-owning view of an entity.
///
/// \details
/// Other than the special member functions, it's undefined behavior to call any member
/// functions other than `try_destroy`, `has_value`, or `operator bool` on an entity without a
/// value.
class Entity final
{
 public:
  TACTILE_DEFAULT_COPY(Entity);
  TACTILE_DEFAULT_MOVE(Entity);

  Entity() noexcept = default;

  Entity(Registry* registry, EntityID id) noexcept;

  ~Entity() noexcept = default;

  void try_destroy() noexcept;

  /// See Registry::add
  template <typename T, typename... Args>
  [[nodiscard]] auto add(Args&&... args) -> T*
  {
    assert(has_value());
    return m_registry->add<T>(m_id, std::forward<Args>(args)...);
  }

  /// See Registry::detach
  template <typename T>
  [[nodiscard]] auto detach() -> std::optional<T>
  {
    assert(has_value());
    return m_registry->detach<T>(m_id);
  }

  /// See Registry::erase
  template <typename T>
  void erase()  // NOLINT: should not be const, it conceptually modifies the entity
  {
    assert(has_value());
    return m_registry->erase<T>(m_id);
  }

  /// See Registry::find
  template <typename T>
  [[nodiscard]] auto find() -> T*
  {
    assert(has_value());
    return m_registry->find<T>(m_id);
  }

  /// See Registry::find
  template <typename T>
  [[nodiscard]] auto find() const -> const T*
  {
    assert(has_value());
    return m_registry->find<T>(m_id);
  }

  /// See Registry::get
  template <typename T>
  [[nodiscard]] auto get() -> T&
  {
    assert(has_value());
    return m_registry->get<T>(m_id);
  }

  /// See Registry::get
  template <typename T>
  [[nodiscard]] auto get() const -> const T&
  {
    assert(has_value());
    return m_registry->get<T>(m_id);
  }

  /// See Registry::has
  template <typename T>
  [[nodiscard]] auto has() const -> bool
  {
    assert(has_value());
    return m_registry->has<T>(m_id);
  }

  /// Returns the associated registry.
  [[nodiscard]]
  auto get_registry() -> Registry&;

  /// Returns the associated registry.
  [[nodiscard]]
  auto get_registry() const -> const Registry&;

  /// Returns the associated identifier.
  [[nodiscard]]
  auto get_id() const noexcept -> EntityID;

  /// Indicates whether the entity has a value.
  [[nodiscard]]
  auto has_value() const noexcept -> bool;

  /// Indicates whether the entity has a value.
  [[nodiscard]]
  explicit operator bool() const noexcept;

 private:
  Registry* m_registry {};
  EntityID m_id {kInvalidEntity};
};

}  // namespace tactile::core
