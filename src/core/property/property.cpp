#include "property.hpp"

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::core {

void property::reset()
{
  m_value.emplace<std::monostate>();
}

void property::set_default(const property_type type)
{
  if (type == property_type::integer)
  {
    set_value<int>();
  }
  else if (type == property_type::floating)
  {
    set_value<double>();
  }
  else if (type == property_type::boolean)
  {
    set_value<bool>();
  }
  else if (type == property_type::string)
  {
    set_value<QString>();
  }
  else if (type == property_type::color)
  {
    set_value<QColor>(Qt::black);
  }
  else if (type == property_type::object)
  {
    set_value<object_ref>();
  }
  else if (type == property_type::file)
  {
    set_value<QFileInfo>();
  }
}

auto property::has_value() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(m_value);
}

auto property::type() const -> maybe<property_type>
{
  if (is<int>())
  {
    return property_type::integer;
  }
  else if (is<double>())
  {
    return property_type::floating;
  }
  else if (is<QString>())
  {
    return property_type::string;
  }
  else if (is<bool>())
  {
    return property_type::boolean;
  }
  else if (is<QFileInfo>())
  {
    return property_type::file;
  }
  else if (is<object_ref>())
  {
    return property_type::object;
  }
  else if (is<QColor>())
  {
    return property_type::color;
  }
  else
  {
    return std::nullopt;
  }
}

auto property::as_string() const -> const QString&
{
  return as<QString>();
}

auto property::as_integer() const -> int
{
  return as<int>();
}

auto property::as_floating() const -> double
{
  return as<double>();
}

auto property::as_boolean() const -> bool
{
  return as<bool>();
}

auto property::as_file() const -> const QFileInfo&
{
  return as<QFileInfo>();
}

auto property::as_object() const -> object_ref
{
  return as<object_ref>();
}

auto property::as_color() const -> const QColor&
{
  return as<QColor>();
}

auto property::is_string() const noexcept -> bool
{
  return type() == property_type::string;
}

auto property::is_integer() const noexcept -> bool
{
  return type() == property_type::integer;
}

auto property::is_floating() const noexcept -> bool
{
  return type() == property_type::floating;
}

auto property::is_boolean() const noexcept -> bool
{
  return type() == property_type::boolean;
}

auto property::is_file() const noexcept -> bool
{
  return type() == property_type::file;
}

auto property::is_object() const noexcept -> bool
{
  return type() == property_type::object;
}

auto property::is_color() const noexcept -> bool
{
  return type() == property_type::color;
}

auto stringify(const property_type type) -> QString
{
  switch (type)
  {
    case property_type::string:
      return TACTILE_QSTRING(u"string");

    case property_type::integer:
      return TACTILE_QSTRING(u"int");

    case property_type::floating:
      return TACTILE_QSTRING(u"float");

    case property_type::boolean:
      return TACTILE_QSTRING(u"bool");

    case property_type::file:
      return TACTILE_QSTRING(u"file");

    case property_type::color:
      return TACTILE_QSTRING(u"color");

    case property_type::object:
      return TACTILE_QSTRING(u"object");

    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

}  // namespace tactile::core
