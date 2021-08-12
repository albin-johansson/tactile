#ifndef RUNE_ECS_EVENTS_KEY_BIND_TRIGGERED_EVENT_HPP
#define RUNE_ECS_EVENTS_KEY_BIND_TRIGGERED_EVENT_HPP

#include "../../aliases/integers.hpp"
#include "../null_entity.hpp"
#include "../ui/ui_key_bind.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \struct key_bind_triggered_event
 *
 * \brief An event that is emitted whenever a key bind is triggered.
 *
 * \since 0.1.0
 */
struct key_bind_triggered_event final
{
  ui_key_bind::entity entity{null<ui_key_bind>()};  ///< The key bind entity.
  uint32 id{};                                      ///< The key bind ID.
};

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_EVENTS_KEY_BIND_TRIGGERED_EVENT_HPP
