#ifndef RUNE_ECS_UI_UI_LINE_HPP
#define RUNE_ECS_UI_UI_LINE_HPP

#include "../entity_type.hpp"
#include "ui_position.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_line_tag;
}  // namespace tags

/**
 * \struct ui_line
 *
 * \brief A component that represents a UI line.
 *
 * \see `ui::make_line()`
 * \see `serialize(auto&, ui_line&)`
 *
 * \since 0.1.0
 */
struct ui_line final
{
  using entity = entity_type<tags::ui_line_tag>;

  ui_position start;  ///< The start point.
  ui_position end;    ///< The end point.
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_line& line)
{
  archive(line.start, line.end);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_LINE_HPP
