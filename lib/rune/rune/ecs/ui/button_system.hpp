#ifndef RUNE_ECS_UI_BUTTON_SYSTEM_HPP
#define RUNE_ECS_UI_BUTTON_SYSTEM_HPP

#include <cassert>        // assert
#include <centurion.hpp>  // mouse, color
#include <entt.hpp>       // registry, dispatcher
#include <string>         // string
#include <utility>        // move

#include "../../aliases/font_id.hpp"
#include "../../aliases/integers.hpp"
#include "../../aliases/maybe.hpp"
#include "../../core/graphics.hpp"
#include "../events/button_pressed_event.hpp"
#include "../null_entity.hpp"
#include "label_system.hpp"
#include "ui_button.hpp"
#include "ui_checkbox.hpp"
#include "ui_foreground.hpp"
#include "ui_grid.hpp"
#include "ui_label.hpp"
#include "ui_menu.hpp"
#include "ui_position.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \struct ui_button_cfg
 *
 * \brief Configuration used when creating button entities.
 *
 * \see `ui_button`
 * \see `ui::add_button()`
 * \see `ui::make_button()`
 *
 * \since 0.1.0
 */
struct ui_button_cfg final
{
  ui_position position;               ///< The position of the button.
  std::string text;                   ///< The button label text.
  uint32 id{};                        ///< User-defined button identifier.
  font_id font{};                     ///< The font used by the label.
  cen::color fg{cen::colors::white};  ///< The label color.
  bool shadow{};                      ///< Should the label feature a shadow?
};

/// \} End of group ecs

namespace ui {

/// \addtogroup ecs
/// \{

/// \name Factory functions
/// \{

/**
 * \brief Adds button components to an existing entity.
 *
 * \details The supplied entity will have the following components added to it.
 * - `ui_button`
 * - `ui_position`
 * - `ui_foreground`
 * - `ui_label`
 * - `ui_label_shadow` (if `ui_button_cfg::shadow` is `true`)
 * - `in_menu`
 *
 * \param registry the registry in which the supplied entity belongs to.
 * \param menu the host menu entity.
 * \param entity the entity to which button components will be added.
 * \param cfg the button configuration that will be used.
 *
 * \since 0.1.0
 */
inline void add_button(entt::registry& registry,
                       ui_menu::entity menu,
                       entt::entity entity,
                       ui_button_cfg cfg)
{
  auto& button = registry.emplace<ui_button>(entity);
  button.id = cfg.id;

  add_label(registry,
            menu,
            entity,
            {.position = cfg.position,
             .text = std::move(cfg.text),
             .font = cfg.font,
             .color = cfg.fg,
             .shadow = cfg.shadow});
}

/**
 * \brief Creates a new button entity and returns it.
 *
 * \details The created entity will feature the following components.
 * - `ui_button`
 * - `ui_position`
 * - `ui_foreground`
 * - `ui_label`
 * - `ui_label_shadow` (if `ui_button_cfg::shadow` is `true`)
 * - `in_menu`
 *
 * \param registry the registry to which a button entity will be added.
 * \param menu the host menu entity.
 * \param cfg the button configuration that will be used.
 *
 * \return the created button entity.
 *
 * \see `ui_button_cfg`
 * \see `add_button()`
 *
 * \since 0.1.0
 */
inline auto make_button(entt::registry& registry, ui_menu::entity menu, ui_button_cfg cfg)
    -> ui_button::entity
{
  const auto entity = ui_button::entity{registry.create()};

  add_button(registry, menu, entity, std::move(cfg));

  return entity;
}

/// \} End of factory functions

/// \} End of group ecs

/// \cond FALSE

namespace detail {

[[nodiscard]] inline auto query_button(entt::registry& registry,
                                       entt::dispatcher& dispatcher,
                                       ui_button::entity entity,
                                       const cen::mouse& mouse) -> bool
{
  auto& button = registry.get<ui_button>(entity);
  if (button.is_enabled && button.is_hovered)
  {
    // TODO enable_cursor

    if (mouse.was_left_button_released())
    {
      dispatcher.trigger<button_pressed_event>(entity, button.id);
      button.is_hovered = false;

      if (auto* checkbox = registry.try_get<ui_checkbox>(entity))
      {
        checkbox->is_checked = !checkbox->is_checked;
      }

      // TODO reset cursor

      return true;
    }
  }

  return false;
}

[[nodiscard]] inline auto update_button_hover(entt::registry& registry,
                                              const cen::mouse& mouse)
    -> maybe<ui_button::entity>
{
  const auto menuEntity = registry.ctx<const active_menu>().menu_entity;
  const auto mousePos = cen::cast<cen::fpoint>(mouse.position());

  for (auto&& [entity, button, position, inMenu] :
       registry.view<ui_button, const ui_position, const in_menu>().each())
  {
    if (inMenu.menu_entity == menuEntity && button.is_visible && button.size)
    {
      const auto bounds = cen::frect{from_grid(position), *button.size};
      button.is_hovered = bounds.contains(mousePos);
      if (button.is_hovered)
      {
        return ui_button::entity{entity};
      }
    }
  }

  return nothing;
}

inline void update_button_bounds(const entt::registry& registry, graphics& gfx)
{
  auto& renderer = gfx.get_renderer();
  for (auto&& [entity, button, label] : registry.view<ui_button, ui_label>().each())
  {
    if (!button.size)
    {
      const auto& font = gfx.get_font(label.font);
      auto size = cen::cast<cen::farea>(font.string_size(label.text).value());

      const auto widthPadding = size.width * 0.5f;
      const auto heightPadding = size.height * 0.5f;

      size.width += widthPadding;
      size.height += heightPadding;

      button.size = size;
      button.text_offset = cen::fpoint{widthPadding / 2.0f, heightPadding / 2.0f};
    }
  }
}

inline void render_buttons(const entt::registry& registry, graphics& gfx)
{
  const auto menuEntity = registry.ctx<active_menu>().menu_entity;
  auto& renderer = gfx.get_renderer();

  const auto view = registry.view<ui_button, ui_position, in_menu>();
  for (auto&& [entity, button, position, inMenu] : view.each())
  {
    if (menuEntity == inMenu.menu_entity)
    {
      assert(button.size);
      const auto rect = cen::frect{from_grid(position), *button.size};

      renderer.set_color(cen::colors::white);
      renderer.draw_rect(rect);

      if (button.is_hovered)
      {
        renderer.set_color(cen::colors::lime);
        renderer.draw_rect(rect);
      }
    }
  }
}

}  // namespace detail

/// \endcond

}  // namespace ui
}  // namespace rune

#endif  // RUNE_ECS_UI_BUTTON_SYSTEM_HPP
