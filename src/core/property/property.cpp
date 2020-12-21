#include "property.hpp"

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

}  // namespace tactile::core
