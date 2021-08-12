#ifndef RUNE_ECS_UI_MENU_SYSTEM_HPP
#define RUNE_ECS_UI_MENU_SYSTEM_HPP

#include <entt.hpp>  // registry
#include <string>    // string
#include <utility>   // move

#include "../../aliases/font_id.hpp"
#include "../../aliases/integers.hpp"
#include "ui_label.hpp"
#include "ui_menu.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

/**
 * \struct ui_menu_cfg
 *
 * \brief Configuration used when creating menu entities.
 *
 * \see `ui_menu`
 * \see `ui::make_menu()`
 *
 * \since 0.1.0
 */
struct ui_menu_cfg final
{
  uint32 id{};             ///< User-defined identifier.
  std::string title;       ///< The menu title.
  font_id font{};          ///< The ID of the title font.
  bool is_blocking{true};  ///< Does the menu block game logic updates?
};

/// \} End of group ecs

namespace ui {

/// \addtogroup ecs
/// \{

/// \name Factory functions
/// \{

/**
 * \brief Creates a new menu entity and returns it.
 *
 * \details The created entity will feature the following components.
 * - `ui_menu`
 * - `ui_label`
 *
 * \param registry the registry to which a menu entity will be added.
 * \param cfg the menu configuration that will be used.
 *
 * \return the created menu entity.
 *
 * \see `ui_menu_cfg`
 *
 * \since 0.1.0
 */
inline auto make_menu(entt::registry& registry, ui_menu_cfg cfg) -> ui_menu::entity
{
  const auto entity = ui_menu::entity{registry.create()};

  auto& menu = registry.emplace<ui_menu>(entity);
  menu.id = cfg.id;
  menu.is_blocking = cfg.is_blocking;

  auto& label = registry.emplace<ui_label>(entity);
  label.text = std::move(cfg.title);
  label.font = cfg.font;

  return entity;
}

/// \} End of factory functions

/// \} End of group ecs

}  // namespace ui

}  // namespace rune

#endif  // RUNE_ECS_UI_MENU_SYSTEM_HPP
