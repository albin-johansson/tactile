// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn_format/gd3_scene_writer.hpp"

#include <iomanip>  // quoted, setprecision
#include <ios>      // boolalpha, fixed
#include <utility>  // move

namespace tactile::godot {

Gd3SceneWriter::Gd3SceneWriter(std::ostream& stream)
  : m_stream {&stream},
    m_key_prefix {}
{
  *m_stream << std::setprecision(3) << std::fixed;
}

auto Gd3SceneWriter::newline() -> Gd3SceneWriter&
{
  *m_stream << '\n';
  return *this;
}

auto Gd3SceneWriter::gd_scene_header(const std::size_t load_steps) -> Gd3SceneWriter&
{
  *m_stream << "[gd_scene load_steps=" << load_steps << " format=2]\n";
  return *this;
}

auto Gd3SceneWriter::gd_resource_header(const std::string_view type,
                                        const std::size_t load_steps) -> Gd3SceneWriter&
{
  *m_stream << "[gd_resource type=" << std::quoted(type) << " load_steps=" << load_steps
            << " format=2]\n";
  return *this;
}

auto Gd3SceneWriter::node_header(const std::string_view name, const std::string_view type)
    -> Gd3SceneWriter&
{
  *m_stream << "[node name=" << std::quoted(name) << " type=" << std::quoted(type) << "]\n";
  return *this;
}

auto Gd3SceneWriter::node_header(const std::string_view name,
                                 const std::string_view type,
                                 const std::string_view parent) -> Gd3SceneWriter&
{
  *m_stream << "[node name=" << std::quoted(name)  //
            << " type=" << std::quoted(type)       //
            << " parent=" << std::quoted(parent) << "]\n";
  return *this;
}

auto Gd3SceneWriter::resource_header() -> Gd3SceneWriter&
{
  *m_stream << "[resource]\n";
  return *this;
}

auto Gd3SceneWriter::ext_resource_header(const ExtResourceId id,
                                         const Gd3ExtResource& resource) -> Gd3SceneWriter&
{
  *m_stream << "[ext_resource path=" << std::quoted(resource.path)  //
            << " type=" << std::quoted(resource.type)               //
            << " id=" << id << "]\n";
  return *this;
}

auto Gd3SceneWriter::sub_resource_header(const std::string_view type, const SubResourceId id)
    -> Gd3SceneWriter&
{
  *m_stream << "[sub_resource type=" << std::quoted(type) << " id=" << id << "]\n";
  return *this;
}

auto Gd3SceneWriter::variable(const std::string_view key, const std::string_view value)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = " << value << '\n';
  return *this;
}

auto Gd3SceneWriter::variable(const std::string_view key, const bool value) -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = " << std::boolalpha << value << '\n';
  return *this;
}

auto Gd3SceneWriter::variable_quoted(const std::string_view key, const std::string_view value)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = " << std::quoted(value) << '\n';
  return *this;
}

auto Gd3SceneWriter::vector2_variable(const std::string_view key, const Int2& vec)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = Vector2( " << vec.x() << ", " << vec.y() << " )\n";
  return *this;
}

auto Gd3SceneWriter::vector2_variable(const std::string_view key, const Float2& vec)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = Vector2( " << vec.x() << ", " << vec.y() << " )\n";
  return *this;
}

auto Gd3SceneWriter::rect2_variable(const std::string_view key, const Int4& rect)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = Rect2( "  //
            << rect.x() << ", "     //
            << rect.y() << ", "     //
            << rect.z() << ", "     //
            << rect.w() << " )\n";
  return *this;
}

auto Gd3SceneWriter::sub_resource_variable(const std::string_view key, const SubResourceId id)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = SubResource( " << id << " )\n";
  return *this;
}

auto Gd3SceneWriter::ext_resource_variable(const std::string_view key, const ExtResourceId id)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = ExtResource( " << id << " )\n";
  return *this;
}

auto Gd3SceneWriter::color_variable(const std::string_view key, const FColor& color)
    -> Gd3SceneWriter&
{
  _emit_key_prefix();
  *m_stream << key << " = Color( " << color.red << ", " << color.green << ", " << color.blue
            << ", " << color.alpha << " )\n";
  return *this;
}

void Gd3SceneWriter::set_key_prefix(std::string prefix)
{
  m_key_prefix = std::move(prefix);
}

void Gd3SceneWriter::_emit_key_prefix() const
{
  if (!m_key_prefix.empty()) {
    *m_stream << m_key_prefix;
  }
}

}  // namespace tactile::godot
