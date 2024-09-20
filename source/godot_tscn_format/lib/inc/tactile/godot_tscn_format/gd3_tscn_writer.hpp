// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>      // size_t
#include <ostream>      // ostream
#include <string>       // string
#include <string_view>  // string_view

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/godot_tscn_format/api.hpp"
#include "tactile/godot_tscn_format/gd3_types.hpp"

namespace tactile::godot {

/**
 * Utility for generating Godot 3 TSCN files.
 */
class TACTILE_TSCN_FORMAT_API Gd3TscnWriter final
{
 public:
  /**
   * Creates a TSCN writer.
   *
   * \param stream The output stream to use, whose lifetime must exceed the writer.
   */
  explicit Gd3TscnWriter(std::ostream& stream);

  /**
   * Outputs a line break.
   *
   * \return
   * The writer itself.
   */
  auto newline() -> Gd3TscnWriter&;

  /**
   * Outputs a "gd_scene" header.
   *
   * \param load_steps The number of load steps.
   *
   * \return
   * The writer itself.
   */
  auto gd_scene_header(std::size_t load_steps) -> Gd3TscnWriter&;

  /**
   * Outputs a "gd_resource" header.
   *
   * \param type       The resource type.
   * \param load_steps The number of load steps.
   *
   * \return
   * The writer itself.
   */
  auto gd_resource_header(std::string_view type, std::size_t load_steps) -> Gd3TscnWriter&;

  /**
   * Outputs a "node" header without a parent node.
   *
   * \param name       The node name.
   * \param type       The node type.
   *
   * \return
   * The writer itself.
   */
  auto node_header(std::string_view name, std::string_view type) -> Gd3TscnWriter&;

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
  auto node_header(std::string_view name,
                   std::string_view type,
                   std::string_view parent) -> Gd3TscnWriter&;

  /**
   * Outputs an "ext_resource" header.
   *
   * \param id       The associated identifier.
   * \param resource The external resource.
   *
   * \return
   * The writer itself.
   */
  auto ext_resource_header(ExtResourceId id, const Gd3ExtResource& resource) -> Gd3TscnWriter&;

  /**
   * Outputs a "sub_resource" header.
   *
   * \param type The resource type.
   * \param id   The associated identifier.
   *
   * \return
   * The writer itself.
   */
  auto sub_resource_header(std::string_view type, SubResourceId id) -> Gd3TscnWriter&;

  /**
   * Outputs a plain key/value pair.
   *
   * \param key   The variable name.
   * \param value The variable value.
   *
   * \return
   * The writer itself.
   */
  auto variable(std::string_view key, std::string_view value) -> Gd3TscnWriter&;

  /**
   * \copydoc variable(std::string_view, std::string_view)
   */
  auto variable(std::string_view key, bool value) -> Gd3TscnWriter&;

  /**
   * Outputs a plain key/value pair with quotes around the value.
   *
   * \param key   The variable name.
   * \param value The variable value.
   *
   * \return
   * The writer itself.
   */
  auto variable_quoted(std::string_view key, std::string_view value) -> Gd3TscnWriter&;

  /**
   * Outputs a "Vector2" variable.
   *
   * \param key The variable name.
   * \param vec The variable value.
   *
   * \return
   * The writer itself.
   */
  auto vector2_variable(std::string_view key, const Int2& vec) -> Gd3TscnWriter&;

  /**
   * \copydoc vector2_variable(std::string_view, const Int2&)
   */
  auto vector2_variable(std::string_view key, const Float2& vec) -> Gd3TscnWriter&;

  /**
   * Outputs a "Rect2" variable.
   *
   * \param key  The variable name.
   * \param rect The variable value.
   *
   * \return
   * The writer itself.
   */
  auto rect2_variable(std::string_view key, const Int4& rect) -> Gd3TscnWriter&;

  /**
   * Outputs a "SubResource" variable.
   *
   * \param key The variable name.
   * \param id  The variable value.
   *
   * \return
   * The writer itself.
   */
  auto sub_resource_variable(std::string_view key, SubResourceId id) -> Gd3TscnWriter&;

  /**
   * Outputs an "ExtResource" variable.
   *
   * \param key The variable name.
   * \param id  The variable value.
   *
   * \return
   * The writer itself.
   */
  auto ext_resource_variable(std::string_view key, ExtResourceId id) -> Gd3TscnWriter&;

  /**
   * Sets the prefix that is prepended to variable keys.
   *
   * \param prefix The new key prefix.
   */
  void set_key_prefix(std::string prefix);

  [[nodiscard]]
  auto get_stream() -> std::ostream&;

 private:
  std::ostream* m_stream;
  std::string m_key_prefix;

  void _emit_key_prefix() const;
};

}  // namespace tactile::godot
