#ifndef RUNE_CORE_GRAPHICS_HPP
#define RUNE_CORE_GRAPHICS_HPP

#include <cassert>        // assert
#include <centurion.hpp>  // window, renderer, texture, font_cache, pixel_format
#include <string>         // string
#include <unordered_map>  // unordered_map
#include <utility>        // forward
#include <vector>         // vector

#include "../aliases/font_id.hpp"
#include "../aliases/integers.hpp"
#include "../aliases/texture_id.hpp"
#include "../aliases/texture_index.hpp"
#include "compiler.hpp"

namespace rune {

/// \addtogroup core
/// \{

/**
 * \class graphics
 *
 * \brief Provides the main graphics API.
 *
 * \details This class provides a renderer, efficient texture handling, font caches for
 * efficient text rendering, pixel format information, etc.
 *
 * \details For reduced memory consumption and redundancy in loaded textures, this class
 * manages a collection of textures that are given unique indices when loaded. These
 * indices literally correspond to indices in an array of textures managed by this class,
 * which results in very fast constant complexity lookup of textures.
 *
 * \details It is safe to derive your own custom graphics context classes from this class.
 * However, you then need to supply your custom graphics type as a template parameter to
 * your engine instance.
 *
 * \since 0.1.0
 */
class graphics
{
 public:
  using size_type = usize;

  /**
   * \brief Creates a graphics context.
   *
   * \tparam T the ownership semantics of the window.
   * \param window the associated game window.
   * \param flags the renderer flags supplied to the `cen::renderer` constructor.
   */
  template <typename T>
  graphics(const cen::basic_window<T>& window, const uint32 flags)
      : m_renderer{window, flags}
      , m_format{window.get_pixel_format()}
  {}

  virtual ~graphics() noexcept = default;

  /// \name Texture handling
  /// \{

  /**
   * \brief Reserves enough memory to store the specified amount of textures.
   *
   * \param capacity the new capacity of textures.
   */
  void reserve(const size_type capacity)
  {
    m_textures.reserve(capacity);
  }

  /**
   * \brief Loads a texture and returns the associated index.
   *
   * \details If a texture with the specified ID has already been loaded, then this
   * function does nothing, and just returns the existing texture index.
   *
   * \param id the unique ID of the texture.
   * \param path the file path of the texture.
   *
   * \return the index of the loaded texture.
   */
  auto load(const texture_id id, const std::string& path) -> texture_index
  {
    if (const auto it = m_indices.find(id); it != m_indices.end())
    {
      return it->second;
    }
    else
    {
      const auto index = m_textures.size();

      m_textures.emplace_back(m_renderer, path);
      m_indices.try_emplace(id, index);

      return texture_index{index};
    }
  }

  /**
   * \brief Returns the texture associated with the specified index.
   *
   * \details This function performs a very fast index lookup for finding the associated
   * texture. This function is not bounds checked in optimized builds, but an assertion
   * will abort the execution of the program in debug builds if an invalid index is used.
   *
   * \pre `index` must be associated with an existing texture.
   *
   * \param index the index of the desired texture.
   *
   * \return the texture associated with the index.
   */
  [[nodiscard]] auto at(const texture_index index) const noexcept(on_msvc())
      -> const cen::texture&
  {
    assert(index < m_textures.size());  // texture_index is unsigned
    return m_textures[index];
  }

  /// \copydoc at()
  [[nodiscard]] auto operator[](const texture_index index) const noexcept(on_msvc())
      -> const cen::texture&
  {
    return at(index);
  }

  /**
   * \brief Indicates whether or not a texture index is associated with a texture.
   *
   * \param index the texture index that will be checked.
   *
   * \return `true` if the texture index is associated with a texture; `false` otherwise.
   */
  [[nodiscard]] auto contains(const texture_index index) const noexcept -> bool
  {
    return index < m_textures.size();
  }

  /**
   * \brief Returns the texture index associated with the specified ID.
   *
   * \param id the ID associated with the texture.
   *
   * \return the index of the specified texture.
   *
   * \throws std::out_of_range if the supplied ID isn't associated with an index.
   */
  [[nodiscard]] auto to_index(const texture_id id) const -> texture_index
  {
    return m_indices.at(id);
  }

  /// \} End of texture handling

  /// \name Font cache handling
  /// \{

  /**
   * \brief Adds a font cache to the graphics context.
   *
   * \tparam Args the types of the font cache constructor arguments.
   *
   * \note Any previous font cache associated with the supplied ID is overwritten.
   *
   * \param id the unique ID that will be associated with the font cache.
   * \param args the arguments that will be forwarded to an appropriate font cache
   * constructor.
   */
  template <typename... Args>
  void emplace_cache(const font_id id, Args&&... args)
  {
    m_caches.insert_or_assign(id, cen::font_cache{std::forward<Args>(args)...});
  }

  /**
   * \brief Returns the font cache associated with the specified ID.
   *
   * \param id the ID associated with the desired font cache.
   *
   * \return the found font cache.
   *
   * \throws std::out_of_range if there is no font cache associated with the ID.
   */
  [[nodiscard]] auto get_cache(const font_id id) -> cen::font_cache&
  {
    return m_caches.at(id);
  }

  /// \copydoc get_cache()
  [[nodiscard]] auto get_cache(const font_id id) const -> const cen::font_cache&
  {
    return m_caches.at(id);
  }

  [[nodiscard]] auto get_font(const font_id id) -> cen::font&
  {
    return get_cache(id).get_font();
  }

  [[nodiscard]] auto get_font(const font_id id) const -> const cen::font&
  {
    return get_cache(id).get_font();
  }

  /**
   * \brief Indicates whether or not the graphics context has a font cache associated with
   * the specified ID.
   *
   * \param id the ID that will be checked.
   *
   * \return `true` if there is a font cache associated with the ID; `false` otherwise.
   */
  [[nodiscard]] auto contains_cache(const font_id id) const -> bool
  {
    return m_caches.contains(id);
  }

  /// \} End of font cache handling

  /**
   * \brief Returns the renderer associated with the graphics context.
   *
   * \return the associated renderer.
   */
  [[nodiscard]] auto get_renderer() noexcept -> cen::renderer&
  {
    return m_renderer;
  }

  /// \copydoc get_renderer()
  [[nodiscard]] auto get_renderer() const noexcept -> const cen::renderer&
  {
    return m_renderer;
  }

  /**
   * \brief Returns the pixel format used by the associated window.
   *
   * \return the associated pixel format.
   */
  [[nodiscard]] auto format() const noexcept -> cen::pixel_format
  {
    return m_format;
  }

 private:
  cen::renderer m_renderer;
  cen::pixel_format m_format;
  std::vector<cen::texture> m_textures;
  std::unordered_map<texture_id, texture_index> m_indices;
  std::unordered_map<size_type, cen::font_cache> m_caches;
};

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_GRAPHICS_HPP
