#include "json_attributes.hpp"

#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // move

#include <nlohmann/json.hpp>

#include "io/maps/json_utils.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_value(const nlohmann::json& json,
                                const std::string_view type,
                                attribute_value& value) -> parse_error
{
  if (type == "string") {
    value = as_string(json, "value").value();
  }
  else if (type == "int") {
    value = as_int(json, "value").value();
  }
  else if (type == "float") {
    value = as_float(json, "value").value();
  }
  else if (type == "bool") {
    value = as_bool(json, "value").value();
  }
  else if (type == "file") {
    std::filesystem::path path = as_string(json, "value").value();
    value = std::move(path);
  }
  else if (type == "object") {
    value = object_t{as_int(json, "value").value()};
  }
  else if (type == "color") {
    const auto hex = as_string(json, "value").value();
    if (const auto color =
            (hex.size() == 9) ? cen::color::from_argb(hex) : cen::color::from_rgb(hex)) {
      value = *color;
    }
    else {
      return parse_error::corrupt_property_value;
    }
  }
  else {
    return parse_error::unsupported_property_type;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_property(const nlohmann::json& json,
                                   ir::attribute_context_data& contextData) -> parse_error
{
  std::string propertyName;

  if (auto name = as_string(json, "name")) {
    propertyName = std::move(*name);
  }
  else {
    return parse_error::no_property_name;
  }

  auto& value = contextData.properties[std::move(propertyName)];

  if (auto type = as_string(json, "type")) {
    if (const auto err = _parse_value(json, *type, value); err != parse_error::none) {
      return err;
    }
  }
  else {
    return parse_error::no_property_type;
  }

  return parse_error::none;
}

}  // namespace

auto parse_properties(const nlohmann::json& json, ir::attribute_context_data& contextData)
    -> parse_error
{
  if (const auto it = json.find("properties"); it != json.end()) {
    for (const auto& [_, value] : it->items()) {
      if (const auto err = _parse_property(value, contextData);
          err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

}  // namespace tactile::parsing