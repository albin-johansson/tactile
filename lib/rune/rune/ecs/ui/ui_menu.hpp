#ifndef RUNE_ECS_UI_UI_MENU_HPP
#define RUNE_ECS_UI_UI_MENU_HPP

#include "../../aliases/integers.hpp"
#include "../entity_type.hpp"
#include "../null_entity.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_menu_tag;
}  // namespace tags

/**
 * \struct ui_menu
 *
 * \brief A component that represents a UI menu.
 *
 * \see `ui::make_menu()`
 * \see `serialize(auto&, ui_menu&)`
 *
 * \since 0.1.0
 */
struct ui_menu final
{
  using entity = entity_type<tags::ui_menu_tag>;

  uint32 id{};         ///< User-defined identifier.
  bool is_blocking{};  ///< Does the menu block game logic updates?
};

struct active_menu final
{
  ui_menu::entity menu_entity{null<ui_menu>()};
};

struct in_menu final
{
  ui_menu::entity menu_entity{null<ui_menu>()};
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_menu& menu)
{
  archive(menu.id, menu.is_blocking);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_MENU_HPP
