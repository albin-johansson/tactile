#ifndef RUNE_ECS_NULL_ENTITY_HPP
#define RUNE_ECS_NULL_ENTITY_HPP

#include <concepts>  // constructible_from
#include <entt.hpp>  // entity, null

namespace rune {

/// \addtogroup ecs
/// \{

// clang-format off

/**
 * \brief Returns a null entity identifier for a component with an `entity` member type.
 *
 * \tparam T the component with a public `entity` member type (based on `entity_type`).
 *
 * \return a null entity identifier.
 *
 * \see `entity_type`
 * \see `nullify()`
 *
 * \since 0.1.0
 */
template <typename T>
[[nodiscard]] constexpr auto null()
    noexcept(noexcept(typename T::entity{entt::entity{entt::null}}))
{
  return typename T::entity{entt::entity{entt::null}};
}

// clang-format on

/**
 * \brief Assigns a null entity identifier to an strong type entity value.
 *
 * \tparam T the strong entity type.
 *
 * \param entity the entity that will be nullified.
 *
 * \see `entity_type`
 * \see `null()`
 *
 * \since 0.1.0
 */
template <std::constructible_from<entt::entity> T>
void nullify(T& entity) noexcept(noexcept(T{entt::entity{entt::null}}))
{
  entity = T{entt::entity{entt::null}};
}

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_NULL_ENTITY_HPP
