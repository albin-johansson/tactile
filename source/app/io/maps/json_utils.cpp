#include "json_utils.hpp"

#include <fstream>  // ifstream, ofstream
#include <iomanip>  // setw
#include <ios>      // ios

#include "core/utils/strings.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/throw.hpp"

namespace tactile {
namespace {

template <typename T>
[[nodiscard]] auto _as(const nlohmann::json& json, const std::string_view name)
    -> maybe<T>
{
  const auto iter = json.find(name);
  if (iter != json.end()) {
    return iter->get<T>();
  }
  else {
    return nothing;
  }
}

}  // namespace

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
      json = convert_to_forward_slashes(value.as_file());
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
  std::ofstream stream{path, std::ios::out | std::ios::trunc};

  if (get_preferences().indent_output()) {
    stream << std::setw(2);
  }

  stream << json;
}

auto read_json(const std::filesystem::path& path) -> maybe<nlohmann::json>
{
  try {
    std::ifstream stream{path, std::ios::in};

    nlohmann::json json;
    stream >> json;

    return json;
  }
  catch (...) {
    return nothing;
  }
}

auto as_string(const nlohmann::json& json, const std::string_view name)
    -> maybe<std::string>
{
  return _as<std::string>(json, name);
}

auto as_int(const nlohmann::json& json, const std::string_view name) -> maybe<int32>
{
  return _as<int32>(json, name);
}

auto as_uint(const nlohmann::json& json, const std::string_view name) -> maybe<uint32>
{
  return _as<uint32>(json, name);
}

auto as_float(const nlohmann::json& json, const std::string_view name) -> maybe<float>
{
  return _as<float>(json, name);
}

auto as_bool(const nlohmann::json& json, const std::string_view name) -> maybe<bool>
{
  return _as<bool>(json, name);
}

}  // namespace tactile
