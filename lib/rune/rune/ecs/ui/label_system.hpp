#ifndef RUNE_ECS_UI_LABEL_SYSTEM_HPP
#define RUNE_ECS_UI_LABEL_SYSTEM_HPP

#include <cassert>        // assert
#include <centurion.hpp>  // color, fpoint, texture
#include <entt.hpp>       // registry
#include <string>         // string
#include <utility>        // move

#include "../../aliases/font_id.hpp"
#include "../../core/graphics.hpp"
#include "../null_entity.hpp"
#include "ui_button.hpp"
#include "ui_foreground.hpp"
#include "ui_grid.hpp"
#include "ui_label.hpp"
#include "ui_menu.hpp"
#include "ui_position.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \struct ui_label_cfg
 *
 * \brief Configuration used when creating label entities.
 *
 * \see `ui::add_label()`
 * \see `ui::make_label()`
 *
 * \since 0.1.0
 */
struct ui_label_cfg final
{
  ui_position position;                  ///< The position of the label.
  std::string text;                      ///< The label text.
  font_id font{};                        ///< The ID of the associated font.
  cen::color color{cen::colors::white};  ///< The label color.
  bool shadow{};                         ///< Does the label have a shadow?
};

/// \} End of group ecs

namespace ui {

/// \addtogroup ecs
/// \{

/// \name Factory functions
/// \{

/**
 * \brief Adds label components to an existing entity.
 *
 * \details The supplied entity will have the following components added to it.
 * - `ui_label`
 * - `ui_label_shadow` (if `ui_label_cfg::shadow` is `true`)
 * - `ui_position`
 * - `ui_foreground`
 * - `in_menu`
 *
 * \param registry the registry in which the supplied entity belongs to.
 * \param menu the host menu entity.
 * \param entity the entity to which label components will be added.
 * \param cfg the label configuration that will be used.
 *
 * \see `make_label()`
 *
 * \since 0.1.0
 */
inline void add_label(entt::registry& registry,
                      const ui_menu::entity menu,
                      const entt::entity entity,
                      ui_label_cfg cfg)
{
  registry.emplace<in_menu>(entity, menu);

  auto& label = registry.emplace<ui_label>(entity);
  label.text = std::move(cfg.text);
  label.font = cfg.font;

  registry.emplace<ui_position>(entity, cfg.position);
  registry.emplace<ui_foreground>(entity, cfg.color);

  if (cfg.shadow)
  {
    registry.emplace<ui_label_shadow>(entity);
  }
}

/**
 * \brief Creates a new label entity and returns it.
 *
 * \details The created entity will feature the following components.
 * - `ui_label`
 * - `ui_label_shadow` (if `ui_label_cfg::shadow` is `true`)
 * - `ui_position`
 * - `ui_foreground`
 * - `in_menu`
 *
 * \param registry the registry to which a label entity will be added.
 * \param menu the host menu entity.
 * \param cfg the label configuration that will be used.
 *
 * \return the created label entity.
 *
 * \see `add_label()`
 *
 * \since 0.1.0
 */
inline auto make_label(entt::registry& registry,
                       const ui_menu::entity menu,
                       ui_label_cfg cfg) -> ui_label::entity
{
  const auto entity = ui_label::entity{registry.create()};

  add_label(registry, menu, entity, std::move(cfg));

  return entity;
}

/// \} End of factory functions

/// \} End of group ecs

/// \cond FALSE

namespace detail {

[[nodiscard]] inline auto render_text(graphics& gfx,
                                      const ui_label& label,
                                      const cen::color& color) -> cen::texture
{
  auto& renderer = gfx.get_renderer();
  renderer.set_color(color);

  const auto& font = gfx.get_font(label.font);
  return renderer.render_blended_utf8(label.text, font);
}

inline void render_label(graphics& gfx,
                         const ui_label& label,
                         const cen::fpoint& position,
                         const cen::color& fg)
{
  auto& renderer = gfx.get_renderer();

  if (!label.texture)
  {
    label.texture = render_text(gfx, label, fg);
  }

  assert(label.texture);
  renderer.render(*label.texture, position);
}

inline void render_shadow(graphics& gfx,
                          const ui_label& label,
                          const ui_label_shadow& shadow,
                          const cen::fpoint& position)
{
  if (!shadow.texture)
  {
    shadow.texture = render_text(gfx, label, cen::colors::black);
  }

  assert(shadow.texture);
  const cen::fpoint offset{static_cast<float>(shadow.offset),
                           static_cast<float>(shadow.offset)};
  gfx.get_renderer().render(*shadow.texture, position + offset);
}

inline void render_labels(const entt::registry& registry, graphics& gfx)
{
  const auto menuEntity = registry.ctx<active_menu>().menu_entity;
  auto& renderer = gfx.get_renderer();

  const auto filter = entt::exclude<ui_button>;
  for (auto&& [entity, label, position, fg, inMenu] :
       registry.view<ui_label, ui_position, ui_foreground, in_menu>(filter).each())
  {
    if (menuEntity == inMenu.menu_entity)
    {
      if (const auto* shadow = registry.try_get<ui_label_shadow>(entity))
      {
        render_shadow(gfx, label, *shadow, from_grid(position));
      }

      render_label(gfx, label, from_grid(position), fg.color);
    }
  }
}

inline void render_button_labels(const entt::registry& registry, graphics& gfx)
{
  const auto menuEntity = registry.ctx<active_menu>().menu_entity;
  auto& renderer = gfx.get_renderer();

  for (auto&& [entity, button, label, position, fg, inMenu] :
       registry.view<ui_button, ui_label, ui_position, ui_foreground, in_menu>().each())
  {
    if (menuEntity == inMenu.menu_entity)
    {
      const auto textPos = from_grid(position) + button.text_offset.value();

      if (const auto* shadow = registry.try_get<ui_label_shadow>(entity))
      {
        render_shadow(gfx, label, *shadow, textPos);
      }

      render_label(gfx, label, textPos, fg.color);
    }
  }
}

}  // namespace detail

/// \endcond

}  // namespace ui

}  // namespace rune

#endif  // RUNE_ECS_UI_LABEL_SYSTEM_HPP
