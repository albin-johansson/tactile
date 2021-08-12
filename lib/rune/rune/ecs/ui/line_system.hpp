#ifndef RUNE_ECS_UI_LINE_SYSTEM_HPP
#define RUNE_ECS_UI_LINE_SYSTEM_HPP

#include <centurion.hpp>  // color
#include <entt.hpp>       // registry

#include "../../core/graphics.hpp"
#include "ui_foreground.hpp"
#include "ui_grid.hpp"
#include "ui_line.hpp"
#include "ui_menu.hpp"
#include "ui_position.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \struct ui_line_cfg
 *
 * \brief Configuration used when creating line entities.
 *
 * \see `ui_line`
 * \see `ui::make_line()`
 *
 * \since 0.1.0
 */
struct ui_line_cfg final
{
  ui_position start;                     ///< The line start position.
  ui_position end;                       ///< The line end position.
  cen::color color{cen::colors::white};  ///< The color of the line.
};

/// \} End of group ecs

namespace ui {

/// \addtogroup ecs
/// \{

/// \name Factory functions
/// \{

/**
 * \brief Creates a new line entity and returns it.
 *
 * \details The created entity will feature the following components.
 * - `ui_line`
 * - `ui_foreground`
 * - `in_menu`
 *
 * \param registry the registry to which a line entity will be added.
 * \param menu the host menu entity.
 * \param cfg the line configuration that will be used.
 *
 * \return the created line entity.
 *
 * \see `ui_line_cfg`
 *
 * \since 0.1.0
 */
inline auto make_line(entt::registry& registry,
                      const ui_menu::entity menu,
                      ui_line_cfg cfg) -> ui_line::entity
{
  const auto entity = ui_line::entity{registry.create()};

  registry.emplace<ui_line>(entity, cfg.start, cfg.end);
  registry.emplace<ui_foreground>(entity, cfg.color);
  registry.emplace<in_menu>(entity, menu);

  return entity;
}

/// \} End of factory functions

/// \} End of group ecs

/// \cond FALSE

namespace detail {

inline void render_lines(const entt::registry& registry, graphics& gfx)
{
  const auto menuEntity = registry.ctx<active_menu>().menu_entity;

  auto& renderer = gfx.get_renderer();
  for (auto&& [entity, line, fg, inMenu] :
       registry.view<ui_line, ui_foreground, in_menu>().each())
  {
    if (menuEntity == inMenu.menu_entity)
    {
      renderer.set_color(fg.color);
      renderer.draw_line(from_grid(line.start), from_grid(line.end));
    }
  }
}

}  // namespace detail

/// \endcond

}  // namespace ui
}  // namespace rune

#endif  // RUNE_ECS_UI_LINE_SYSTEM_HPP
