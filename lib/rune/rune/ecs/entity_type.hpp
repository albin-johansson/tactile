#ifndef RUNE_ECS_ENTITY_TYPE_HPP
#define RUNE_ECS_ENTITY_TYPE_HPP

#include <entt.hpp>   // entity
#include <nenya.hpp>  // strong_type

namespace rune {

/**
 * \typedef entity_type
 *
 * \brief Alias for creating strong types with `entt::entity` as the underlying type.
 *
 * \ingroup ecs
 *
 * \tparam T the unique tag type for the strong type alias.
 *
 * \see `null()`
 * \see `nullify()`
 *
 * \since 0.1.0
 */
template <typename T>
using entity_type = nenya::strong_type<entt::entity, T>;

}  // namespace rune

#endif  // RUNE_ECS_ENTITY_TYPE_HPP
