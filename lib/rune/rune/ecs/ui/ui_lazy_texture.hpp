#ifndef RUNE_ECS_UI_UI_LAZY_TEXTURE_HPP
#define RUNE_ECS_UI_UI_LAZY_TEXTURE_HPP

#include <centurion.hpp>  // surface, texture

#include "../../aliases/maybe.hpp"
#include "../entity_type.hpp"

namespace rune {

/// \addtogroup ecs
/// \{

namespace tags {
struct ui_lazy_texture_tag;
}  // namespace tags

/**
 * \struct ui_lazy_texture
 *
 * \brief A component that represents a lazily initialized texture.
 *
 * \see `ui::make_lazy_texture()`
 *
 * \since 0.1.0
 */
struct ui_lazy_texture final
{
  using entity = entity_type<tags::ui_lazy_texture_tag>;

  cen::surface source;                  ///< The source image data.
  mutable maybe<cen::texture> texture;  ///< Lazily initialized texture.
};

/// \} End of group ecs

}  // namespace rune

#endif  // RUNE_ECS_UI_UI_LAZY_TEXTURE_HPP
