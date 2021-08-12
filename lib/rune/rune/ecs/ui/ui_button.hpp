#ifndef RUNE_ECS_UI_UI_BUTTON_HPP
#define RUNE_ECS_UI_UI_BUTTON_HPP

#include <centurion.hpp>  // farea, fpoint

#include "../../aliases/integers.hpp"
#include "../../aliases/maybe.hpp"
#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_button_tag;
}  // namespace tags

/**
 * \struct ui_button
 *
 * \brief A component that represents a UI button, i.e. a control that can be pressed.
 *
 * \see `ui::add_button()`
 * \see `ui::make_button()`
 * \see `serialize(auto&, ui_button&)`
 *
 * \since 0.1.0
 */
struct ui_button final
{
  using entity = entity_type<tags::ui_button_tag>;

  uint32 id{};                             ///< User-defined identifier.
  mutable maybe<cen::fpoint> text_offset;  ///< Lazily initialized offset for the label.
  mutable maybe<cen::farea> size;          ///< Lazily initialized button size.
  bool is_enabled{true};   ///< Is the button enabled, i.e. can it be pressed?
  bool is_visible{true};   ///< Is the button visible?
  bool is_hovered{false};  ///< Is the button currently hovered?
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_button& b)
{
  archive(b.id, b.text_offset, b.size, b.is_enabled, b.is_visible, b.is_hovered);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_BUTTON_HPP
