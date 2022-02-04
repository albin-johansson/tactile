#include "attribute_value.hpp"

#include "throw.hpp"

namespace tactile {

auto stringify(const attribute_type type) -> const char*
{
  switch (type) {
    case attribute_type::string:
      return "string";

    case attribute_type::integer:
      return "int";

    case attribute_type::floating:
      return "float";

    case attribute_type::boolean:
      return "bool";

    case attribute_type::file:
      return "file";

    case attribute_type::color:
      return "color";

    case attribute_type::object:
      return "object";

    default:
      throw_traced(tactile_error{"Invalid attribute type!"});
  }
}

auto operator<<(std::ostream& stream, const attribute_type type) -> std::ostream&
{
  return stream << stringify(type);
}

void attribute_value::reset_to_default(const attribute_type type)
{
  if (type == attribute_type::integer) {
    set_value<integer_type>(0);
  }
  else if (type == attribute_type::floating) {
    set_value<float_type>(0);
  }
  else if (type == attribute_type::boolean) {
    set_value<bool>(false);
  }
  else if (type == attribute_type::string) {
    set_value<string_type>(string_type{});
  }
  else if (type == attribute_type::color) {
    set_value<color_type>(cen::colors::black);
  }
  else if (type == attribute_type::object) {
    set_value<object_t>(object_t{});
  }
  else if (type == attribute_type::file) {
    set_value<file_type>(file_type{});
  }
  else {
    throw_traced(tactile_error{"Invalid attribute type!"});
  }
}

auto attribute_value::has_default_value() const -> bool
{
  if (const auto* str = std::get_if<string_type>(&mValue)) {
    return str->empty();
  }
  else if (const auto* i = std::get_if<integer_type>(&mValue)) {
    return *i == 0;
  }
  else if (const auto* f = std::get_if<float_type>(&mValue)) {
    return *f == 0;
  }
  else if (const auto* b = std::get_if<bool>(&mValue)) {
    return !*b;
  }
  else if (const auto* path = std::get_if<file_type>(&mValue)) {
    return path->empty();
  }
  else if (const auto* obj = std::get_if<object_t>(&mValue)) {
    return *obj == object_t{};
  }
  else if (const auto* color = std::get_if<color_type>(&mValue)) {
    return *color == cen::colors::black;
  }
  else {
    throw_traced(tactile_error{"Invalid property type!"});
  }
}

auto attribute_value::as_string() const -> const string_type&
{
  if (const auto* str = get_if<string_type>()) {
    return *str;
  }
  else {
    throw_traced(tactile_error{"Attribute was not a string!"});
  }
}

auto attribute_value::as_int() const -> integer_type
{
  if (const auto* i = get_if<integer_type>()) {
    return *i;
  }
  else {
    throw_traced(tactile_error{"Attribute was not an integer!"});
  }
}

auto attribute_value::as_float() const -> float_type
{
  if (const auto* f = get_if<float_type>()) {
    return *f;
  }
  else {
    throw_traced(tactile_error{"Attribute was not a float!"});
  }
}

auto attribute_value::as_bool() const -> bool
{
  if (const auto* b = get_if<bool>()) {
    return *b;
  }
  else {
    throw_traced(tactile_error{"Attribute was not a boolean!"});
  }
}

auto attribute_value::as_file() const -> const file_type&
{
  if (const auto* file = get_if<file_type>()) {
    return *file;
  }
  else {
    throw_traced(tactile_error{"Attribute was not a file!"});
  }
}

auto attribute_value::as_object() const -> object_t
{
  if (const auto* obj = get_if<object_t>()) {
    return *obj;
  }
  else {
    throw_traced(tactile_error{"Attribute was not an object reference!"});
  }
}

auto attribute_value::as_color() const -> const color_type&
{
  if (const auto* color = get_if<color_type>()) {
    return *color;
  }
  else {
    throw_traced(tactile_error{"Attribute was not a color!"});
  }
}

auto operator<<(std::ostream& stream, const attribute_value& value) -> std::ostream&
{
  switch (value.type()) {
    case attribute_type::string:
      return stream << value.as_string();

    case attribute_type::integer:
      return stream << value.as_int();

    case attribute_type::floating:
      return stream << value.as_float();

    case attribute_type::boolean:
      return stream << value.as_bool();

    case attribute_type::file:
      return stream << value.as_file();

    case attribute_type::color:
      return stream << value.as_color().as_rgba();

    case attribute_type::object:
      return stream << "object '" << value.as_object() << "'";

    default:
      throw_traced(tactile_error{"Invalid attribute type!"});
  }
}

}  // namespace tactile