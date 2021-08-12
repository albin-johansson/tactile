#ifndef RUNE_ECS_UI_UI_LABEL_HPP
#define RUNE_ECS_UI_UI_LABEL_HPP

#include <centurion.hpp>  // texture
#include <string>         // string

#include "../../aliases/font_id.hpp"
#include "../../aliases/maybe.hpp"
#include "../entity_type.hpp"
#include "ui_lazy_texture.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_label_tag;
struct ui_label_shadow_tag;
}  // namespace tags

/**
 * \struct ui_label
 *
 * \brief A component that represents a UI label.
 *
 * \see `ui::make_label()`
 * \see `serialize(auto&, ui_label&)`
 *
 * \since 0.1.0
 */
struct ui_label final
{
  using entity = entity_type<tags::ui_label_tag>;

  std::string text;                     ///< The label text.
  font_id font{};                       ///< The associated font ID.
  mutable maybe<cen::texture> texture;  ///< Lazily initialized label texture.
};

/**
 * \struct ui_label_shadow
 *
 * \brief A component that represents a UI label shadow.
 *
 * \see `serialize(auto&, ui_label_shadow&)`
 *
 * \since 0.1.0
 */
struct ui_label_shadow final
{
  using entity = entity_type<tags::ui_label_shadow_tag>;

  mutable maybe<cen::texture> texture;  ///< Lazily initialized shadow texture.
  int offset{1};                        ///< The shadow offset.
};

/// \name Serialization
/// \{

void serialize(auto& archive, ui_label& label)
{
  archive(label.text, label.font);
}

void serialize(auto& archive, ui_label_shadow& shadow)
{
  archive(shadow.offset);
}

/// \} End of serialization

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_LABEL_HPP
