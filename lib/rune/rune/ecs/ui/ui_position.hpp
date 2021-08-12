#ifndef RUNE_ECS_UI_UI_POSITION_HPP
#define RUNE_ECS_UI_UI_POSITION_HPP

#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_position_tag;
}  // namespace tags

struct ui_position final
{
  using entity = entity_type<tags::ui_position_tag>;

  float row{};
  float col{};
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_position& position)
{
  archive(position.row, position.col);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_POSITION_HPP
