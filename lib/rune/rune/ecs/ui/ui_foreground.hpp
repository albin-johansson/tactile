#ifndef RUNE_ECS_UI_UI_FOREGROUND_HPP
#define RUNE_ECS_UI_UI_FOREGROUND_HPP

#include <centurion.hpp>  // color

#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_foreground_tag;
}  // namespace tags

/**
 * \struct ui_foreground
 *
 * \brief A component that represents a foreground color.
 *
 * \see `serialize(auto&, ui_foreground&)`
 *
 * \since 0.1.0
 */
struct ui_foreground final
{
  using entity = entity_type<tags::ui_foreground_tag>;

  cen::color color;  ///< The foreground color.
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_foreground& fg)
{
  archive(fg.color);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_FOREGROUND_HPP
