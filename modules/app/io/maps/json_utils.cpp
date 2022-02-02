#include "json_utils.hpp"

#include <fstream>  // ofstream
#include <iomanip>  // setw
#include <ios>      // ios

#include "io/persistence/preferences.hpp"
#include "throw.hpp"

namespace tactile {

void to_json(nlohmann::json& json, const attribute_value& value)
{
  switch (value.type()) {
    case attribute_type::string:
      json = value.as_string();
      break;

    case attribute_type::integer:
      json = value.as_int();
      break;

    case attribute_type::floating:
      json = value.as_float();
      break;

    case attribute_type::boolean:
      json = value.as_bool();
      break;

    case attribute_type::file:
      json = value.as_file();
      break;

    case attribute_type::color:
      json = value.as_color().as_argb();
      break;

    case attribute_type::object:
      json = value.as_object();
      break;

    default:
      throw_traced(tactile_error{"Invalid attribute type!"});
  }
}

void write_json(const nlohmann::json& json, const std::filesystem::path& path)
{
  std::ofstream stream{path, std::ios::out};

  if (get_preferences().indent_output()) {
    stream << std::setw(2);
  }

  stream << json;
}

}  // namespace tactile
