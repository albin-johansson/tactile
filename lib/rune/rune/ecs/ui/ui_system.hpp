#ifndef RUNE_ECS_UI_UI_SYSTEM_HPP
#define RUNE_ECS_UI_UI_SYSTEM_HPP

#include <entt.hpp>  // registry, dispatcher

#include "../../core/graphics.hpp"
#include "../../core/input.hpp"
#include "button_system.hpp"
#include "key_bind_system.hpp"
#include "label_system.hpp"
#include "lazy_texture_system.hpp"
#include "line_system.hpp"
#include "ui_key_bind.hpp"
#include "ui_menu.hpp"

namespace rune::ui {

/// \addtogroup ecs
/// \{

/**
 * \brief Updates the state of the UI.
 *
 * \details This is the only UI-function you need to call in your `handle_input()`
 * function in order to use the UI framework.
 *
 * \param registry the registry that contains all of the UI entities.
 * \param dispatcher the event dispatcher that will be used.
 * \param input the current input state.
 *
 * \since 0.1.0
 */
inline void update(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const input& input)
{
  if (const auto button = detail::update_button_hover(registry, input.mouse))
  {
    if (detail::query_button(registry, dispatcher, *button, input.mouse))
    {
      // TODO

      return;
    }
  }

  detail::update_key_binds(registry, dispatcher, input);
}

/**
 * \brief Renders the currently active UI components.
 *
 * \param registry the registry that contains all of the UI entities.
 * \param gfx the graphics context that will be used for rendering.
 *
 * \see `debug()`
 *
 * \since 0.1.0
 */
inline void render(const entt::registry& registry, graphics& gfx)
{
  detail::update_button_bounds(registry, gfx);
  detail::update_lazy_textures(registry, gfx);

  detail::render_lines(registry, gfx);
  detail::render_buttons(registry, gfx);
  detail::render_labels(registry, gfx);
  detail::render_button_labels(registry, gfx);
}

/**
 * \brief Renders debug information for the currently active UI components.
 *
 * \details This function is intended to be used to aid debugging UI aspects such as
 * layouts or alignments.
 *
 * \param registry the registry that contains all of the UI entities.
 * \param gfx the graphics context that will be used for rendering.
 *
 * \see `render()`
 *
 * \since 0.1.0
 */
inline void debug(const entt::registry& registry, graphics& gfx)
{
  const auto menuEntity = registry.ctx<const active_menu>().menu_entity;
  const auto& menu = registry.get<ui_menu>(menuEntity);

  if (menu.is_blocking)
  {
    auto& renderer = gfx.get_renderer();
    renderer.set_color(cen::colors::light_gray.with_alpha(50));

    const auto [logicalWidth, logicalHeight] = renderer.logical_size();

    // TODO
  }
}

/// \} End of group ecs

}  // namespace rune::ui

#endif  // RUNE_ECS_UI_UI_SYSTEM_HPP
