#ifndef RUNE_ECS_UI_CHECKBOX_HPP
#define RUNE_ECS_UI_CHECKBOX_HPP

#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_checkbox_tag;
}  // namespace tags

/**
 * \struct ui_checkbox
 *
 * \brief A component that represents a UI button, i.e. a control that can be pressed.
 *
 * \see `serialize(auto&, ui_checkbox&)`
 *
 * \since 0.1.0
 */
struct ui_checkbox final
{
  using entity = entity_type<tags::ui_checkbox_tag>;

  bool is_checked{};
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_checkbox& cb)
{
  archive(cb.is_checked);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_CHECKBOX_HPP
