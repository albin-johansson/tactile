// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>     // invocable
#include <cstddef>      // size_t
#include <ostream>      // ostream
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/base/meta/color.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/godot_tscn_format/api.hpp"
#include "tactile/godot_tscn_format/gd3_types.hpp"

namespace tactile::godot {

/**
 * Utility for emitting Godot 3 scene files.
 */
class TACTILE_GODOT_API Gd3SceneWriter final
{
 public:
  /**
   * Creates a scene writer.
   *
   * \param stream The output stream to use, whose lifetime must exceed the writer.
   */
  explicit Gd3SceneWriter(std::ostream& stream);

  /**
   * Outputs a line break.
   *
   * \return
   * The writer itself.
   */
  auto newline() -> Gd3SceneWriter&;

  /**
   * Outputs a "gd_scene" header.
   *
   * \param load_steps The number of load steps.
   *
   * \return
   * The writer itself.
   */
  auto gd_scene_header(std::size_t load_steps) -> Gd3SceneWriter&;

  /**
   * Outputs a "gd_resource" header.
   *
   * \param type       The resource type.
   * \param load_steps The number of load steps.
   *
   * \return
   * The writer itself.
   */
  auto gd_resource_header(std::string_view type, std::size_t load_steps) -> Gd3SceneWriter&;

  /**
   * Outputs a "node" header without a parent node.
   *
   * \param name       The node name.
   * \param type       The node type.
   *
   * \return
   * The writer itself.
   */
  auto node_header(std::string_view name, std::string_view type) -> Gd3SceneWriter&;

  /**
   * Outputs a "node" header.
   *
   * \param name   The node name.
   * \param type   The node type.
   * \param parent The parent node name.
   *
   * \return
   * The writer itself.
   */
  auto node_header(std::string_view name, std::string_view type, std::string_view parent)
      -> Gd3SceneWriter&;

  /**
   * Outputs a "resource" header.
   *
   * \return
   * The writer itself.
   */
  auto resource_header() -> Gd3SceneWriter&;

  /**
   * Outputs an "ext_resource" header.
   *
   * \param id       The associated identifier.
   * \param resource The external resource.
   *
   * \return
   * The writer itself.
   */
  auto ext_resource_header(ExtResourceId id, const Gd3ExtResource& resource)
      -> Gd3SceneWriter&;

  /**
   * Outputs a "sub_resource" header.
   *
   * \param type The resource type.
   * \param id   The associated identifier.
   *
   * \return
   * The writer itself.
   */
  auto sub_resource_header(std::string_view type, SubResourceId id) -> Gd3SceneWriter&;

  /**
   * Outputs a plain key/value pair.
   *
   * \param key   The variable name.
   * \param value The variable value.
   *
   * \return
   * The writer itself.
   */
  auto variable(std::string_view key, std::string_view value) -> Gd3SceneWriter&;

  /**
   * \copydoc variable(std::string_view, std::string_view)
   */
  auto variable(const std::string_view key, const char* value) -> Gd3SceneWriter&
  {
    return variable(key, std::string_view {value});
  }

  /**
   * \copydoc variable(std::string_view, std::string_view)
   */
  auto variable(std::string_view key, bool value) -> Gd3SceneWriter&;

  /**
   * Outputs a plain key/value pair with quotes around the value.
   *
   * \param key   The variable name.
   * \param value The variable value.
   *
   * \return
   * The writer itself.
   */
  auto variable_quoted(std::string_view key, std::string_view value) -> Gd3SceneWriter&;

  /**
   * Outputs a "Vector2" variable.
   *
   * \param key The variable name.
   * \param vec The variable value.
   *
   * \return
   * The writer itself.
   */
  auto vector2_variable(std::string_view key, const Int2& vec) -> Gd3SceneWriter&;

  /**
   * \copydoc vector2_variable(std::string_view, const Int2&)
   */
  auto vector2_variable(std::string_view key, const Float2& vec) -> Gd3SceneWriter&;

  /**
   * Outputs a "Rect2" variable.
   *
   * \param key  The variable name.
   * \param rect The variable value.
   *
   * \return
   * The writer itself.
   */
  auto rect2_variable(std::string_view key, const Int4& rect) -> Gd3SceneWriter&;

  /**
   * Outputs a "SubResource" variable.
   *
   * \param key The variable name.
   * \param id  The variable value.
   *
   * \return
   * The writer itself.
   */
  auto sub_resource_variable(std::string_view key, SubResourceId id) -> Gd3SceneWriter&;

  /**
   * Outputs an "ExtResource" variable.
   *
   * \param key The variable name.
   * \param id  The variable value.
   *
   * \return
   * The writer itself.
   */
  auto ext_resource_variable(std::string_view key, ExtResourceId id) -> Gd3SceneWriter&;

  /**
   * Outputs a "Color" variable.
   *
   * \param key   The variable name.
   * \param color The variable value.
   *
   * \return
   * The writer itself.
   */
  auto color_variable(std::string_view key, const FColor& color) -> Gd3SceneWriter&;

  /**
   * Outputs a variable with elements from a given range.
   *
   * \tparam T       The type of the range.
   * \tparam Emitter The type of the custom emitter.
   *
   * \param key     The variable name.
   * \param type    The variable type.
   * \param range   The range of elements to include.
   * \param emitter The function object used to output the range elements.
   *
   * \return
   * The writer itself.
   */
  template <typename T, std::invocable<std::ostream&, const typename T::value_type&> Emitter>
  auto sequence_variable(const std::string_view key,
                         const std::string_view type,
                         const T& range,
                         const Emitter& emitter) -> Gd3SceneWriter&
  {
    _emit_key_prefix();
    *m_stream << key << " = " << type << "( ";

    std::size_t index {0};
    for (const auto& elem : range) {
      if (index != 0) {
        *m_stream << ", ";
      }

      emitter(*m_stream, elem);

      ++index;
    }

    *m_stream << " )\n";

    return *this;
  }

  /**
   * Outputs a variable with elements from a given range, using operator<< to output elements.
   *
   * \tparam T The type of the range.
   *
   * \param key   The variable name.
   * \param type  The variable type.
   * \param range The range of elements to include.
   *
   * \return
   * The writer itself.
   */
  template <typename T>
  auto sequence_variable(const std::string_view key,
                         const std::string_view type,
                         const T& range) -> Gd3SceneWriter&
  {
    const auto streamer = [](std::ostream& stream, const typename T::value_type& elem) {
      stream << elem;
    };

    return sequence_variable(key, type, range, streamer);
  }

  /**
   * Sets the prefix that is prepended to variable keys.
   *
   * \param prefix The new key prefix.
   */
  void set_key_prefix(std::string prefix);

 private:
  std::ostream* m_stream;
  std::string m_key_prefix;

  void _emit_key_prefix() const;
};

}  // namespace tactile::godot
