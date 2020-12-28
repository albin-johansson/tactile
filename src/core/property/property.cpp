#include "property.hpp"

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::core {

void property::reset()
{
  m_value.emplace<std::monostate>();
}

void property::set_default(const type t)
{
  if (t == core::property::integer) {
    set_value<int>();

  } else if (t == core::property::floating) {
    set_value<double>();

  } else if (t == core::property::boolean) {
    set_value<bool>();

  } else if (t == core::property::string) {
    set_value<QString>();

  } else if (t == core::property::color) {
    set_value<QColor>(Qt::black);

  } else if (t == core::property::object) {
    set_value<object_ref>();

  } else if (t == core::property::file) {
    set_value<QFileInfo>();
  }
}

auto property::has_value() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(m_value);
}

auto property::get_type() const -> maybe<property::type>
{
  if (is<int>()) {
    return integer;

  } else if (is<double>()) {
    return floating;

  } else if (is<QString>()) {
    return string;

  } else if (is<bool>()) {
    return boolean;

  } else if (is<QFileInfo>()) {
    return file;

  } else if (is<object_ref>()) {
    return object;

  } else if (is<QColor>()) {
    return color;

  } else {
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
  return get_type() == string;
}

auto property::is_integer() const noexcept -> bool
{
  return get_type() == integer;
}

auto property::is_floating() const noexcept -> bool
{
  return get_type() == floating;
}

auto property::is_boolean() const noexcept -> bool
{
  return get_type() == boolean;
}

auto property::is_file() const noexcept -> bool
{
  return get_type() == file;
}

auto property::is_object() const noexcept -> bool
{
  return get_type() == object;
}

auto property::is_color() const noexcept -> bool
{
  return get_type() == color;
}

auto stringify(const property::type type) -> QString
{
  switch (type) {
    case property::string:
      return TACTILE_QSTRING(u"string");

    case property::integer:
      return TACTILE_QSTRING(u"int");

    case property::floating:
      return TACTILE_QSTRING(u"float");

    case property::boolean:
      return TACTILE_QSTRING(u"bool");

    case property::file:
      return TACTILE_QSTRING(u"file");

    case property::color:
      return TACTILE_QSTRING(u"color");

    case property::object:
      return TACTILE_QSTRING(u"object");

    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

}  // namespace tactile::core
