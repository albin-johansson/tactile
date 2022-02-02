#include "yaml_utils.hpp"

#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile {

auto operator<<(YAML::Emitter& emitter, const attribute_value& value) -> YAML::Emitter&
{
  switch (value.type()) {
    case attribute_type::string:
      emitter << value.as_string();
      break;

    case attribute_type::integer:
      emitter << value.as_int();
      break;

    case attribute_type::floating:
      emitter << value.as_float();
      break;

    case attribute_type::boolean:
      emitter << value.as_bool();
      break;

    case attribute_type::file:
      emitter << value.as_file().c_str();
      break;

    case attribute_type::color:
      emitter << value.as_color().as_rgba();
      break;

    case attribute_type::object:
      emitter << value.as_object();
      break;

    default:
      throw_traced(tactile_error{"Invalid attribute type!"});
  }

  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const attribute_type type) -> YAML::Emitter&
{
  return emitter << stringify(type);
}

}  // namespace tactile
