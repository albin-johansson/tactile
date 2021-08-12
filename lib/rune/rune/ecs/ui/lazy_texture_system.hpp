#ifndef RUNE_ECS_UI_LAZY_TEXTURE_SYSTEM_HPP
#define RUNE_ECS_UI_LAZY_TEXTURE_SYSTEM_HPP

#include <centurion.hpp>  // texture, surface
#include <entt.hpp>       // registry

#include "../../aliases/str.hpp"
#include "../../core/graphics.hpp"
#include "ui_lazy_texture.hpp"

namespace rune::ui {

/// \addtogroup ecs
/// \{

/// \name Factory functions
/// \{

/**
 * \brief Creates a new lazy texture entity and returns it.
 *
 * \details The created entity will feature the following components.
 * - `ui_lazy_texture`
 *
 * \param registry the registry to which a lazy texture entity will be added.
 * \param image the file path of the source image.
 *
 * \return the created lazy texture entity.
 *
 * \since 0.1.0
 */
inline auto make_lazy_texture(entt::registry& registry, const str image)
    -> ui_lazy_texture::entity
{
  const auto entity = ui_lazy_texture::entity{registry.create()};

  registry.emplace<ui_lazy_texture>(entity, cen::surface{image});

  return entity;
}

/// \} End of factory functions

/// \} End of group ecs

/// \cond FALSE

namespace detail {

inline void update_lazy_textures(const entt::registry& registry, graphics& gfx)
{
  for (auto&& [entity, lazy] : registry.view<ui_lazy_texture>().each())
  {
    if (!lazy.texture)
    {
      lazy.texture = cen::texture{gfx.get_renderer(), lazy.source};
    }
  }
}

}  // namespace detail

/// \endcond

}  // namespace rune::ui

#endif  // RUNE_ECS_UI_LAZY_TEXTURE_SYSTEM_HPP
