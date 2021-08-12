#ifndef RUNE_ECS_UI_KEY_BIND_SYSTEM_HPP
#define RUNE_ECS_UI_KEY_BIND_SYSTEM_HPP

#include <centurion.hpp>  // scan_code, key_mod
#include <entt.hpp>       // registry, dispatcher

#include "../../aliases/integers.hpp"
#include "../../core/input.hpp"
#include "../events/key_bind_triggered_event.hpp"
#include "ui_key_bind.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \struct ui_key_bind_cfg
 *
 * \brief Configuration used when creating key bind entities.
 *
 * \see `ui_key_bind`
 * \see `ui::make_key_bind()`
 *
 * \since 0.1.0
 */
struct ui_key_bind_cfg final
{
  uint32 id{};                                 ///< User-defined identifier.
  cen::scan_code key;                          ///< The associated key.
  cen::key_mod modifiers{cen::key_mod::none};  ///< The required key modifiers.
};

/// \} End of group ecs

namespace ui {

/// \addtogroup ecs
/// \{

/// \name Factory functions
/// \{

/**
 * \brief Creates a new key bind entity and returns it.
 *
 * \details The created entity will feature the following components.
 * - `ui_key_bind`
 *
 * \param registry the registry to which a key bind entity will be added.
 * \param cfg the key bind configuration that will be used.
 *
 * \return the created key bind entity.
 *
 * \since 0.1.0
 */
inline auto make_key_bind(entt::registry& registry, const ui_key_bind_cfg cfg)
    -> ui_key_bind::entity
{
  const auto entity = ui_key_bind::entity{registry.create()};

  auto& bind = registry.emplace<ui_key_bind>(entity);
  bind.id = cfg.id;
  bind.key = cfg.key;
  bind.modifiers = cfg.modifiers;

  return entity;
}

/// \} End of factory functions

/// \} End of group ecs

/// \cond FALSE

namespace detail {

inline void update_key_binds(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const input& input)
{
  const auto state = cen::get_modifiers();

  // We don't care about these modifiers (they're never used in key binds)
  const auto subset = state & ~(cen::key_mod::num | cen::key_mod::caps);
  cen::set_modifiers(subset);

  for (auto&& [entity, bind] : registry.view<const ui_key_bind>().each())
  {
    if (input.keyboard.just_released(bind.key) &&
        cen::keyboard::is_only_active(bind.modifiers))
    {
      dispatcher.trigger<key_bind_triggered_event>(ui_key_bind::entity{entity}, bind.id);
      return;
    }
  }

  cen::set_modifiers(state);
}

}  // namespace detail

/// \endcond

}  // namespace ui
}  // namespace rune

#endif  // RUNE_ECS_UI_KEY_BIND_SYSTEM_HPP
