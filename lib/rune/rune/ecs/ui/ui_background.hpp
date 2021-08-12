#ifndef RUNE_ECS_UI_UI_BACKGROUND_HPP
#define RUNE_ECS_UI_UI_BACKGROUND_HPP

#include <centurion.hpp>  // color

#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_background_tag;
}  // namespace tags

/**
 * \struct ui_background
 *
 * \brief A component that represents a background color.
 *
 * \see `serialize(auto&, ui_background&)`
 *
 * \since 0.1.0
 */
struct ui_background final
{
  using entity = entity_type<tags::ui_background_tag>;

  cen::color color;  ///< The background color.
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_background& bg)
{
  archive(bg.color);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_BACKGROUND_HPP
