#ifndef RUNE_ECS_UI_UI_KEY_BIND_HPP
#define RUNE_ECS_UI_UI_KEY_BIND_HPP

#include <centurion.hpp>  // scan_code

#include "../../aliases/integers.hpp"
#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_keybind_tag;
}  // namespace tags

/**
 * \struct ui_key_bind
 *
 * \brief A component that represents a key bind, that emits an event when triggered.
 *
 * \see `ui::make_key_bind()`
 * \see `serialize(auto&, ui_key_bind&)`
 *
 * \since 0.1.0
 */
struct ui_key_bind final
{
  using entity = entity_type<tags::ui_keybind_tag>;

  uint32 id{};                                 ///< User-defined identifier.
  cen::scan_code key;                          ///< The associated key.
  cen::key_mod modifiers{cen::key_mod::none};  ///< The associated key modifiers.
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_key_bind& bind)
{
  archive(bind.id, bind.key, bind.modifiers);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_KEY_BIND_HPP
