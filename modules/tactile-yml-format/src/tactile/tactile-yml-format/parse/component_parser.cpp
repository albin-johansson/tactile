// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/component_parser.hpp"

#include <utility>  // move

#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"
#include "tactile/tactile-yml-format/parse/property_parser.hpp"

namespace tactile::yml_format {

auto parse_attached_component_attribute(const YAML::Node& value_node,
                                        const ir::Component& prototype)
    -> Result<ir::NamedAttribute>
{
  ir::NamedAttribute attribute {};
  return parse_to(value_node, "name", attribute.name)
      .and_then([&]() -> Result<Attribute> {
        if (const auto value_attribute = value_node["value"]) {
          if (const auto attr_type = get_attribute_type(prototype, attribute.name)) {
            return parse_property_value(value_attribute, *attr_type);
          }

          return unexpected(
              make_save_format_error(SaveFormatError::kUnsupportedPropertyType));
        }

        return unexpected(make_save_format_error(SaveFormatError::kPropertyWithoutValue));
      })
      .and_then([&](Attribute&& value) {
        attribute.value = std::move(value);
        return kOK;
      })
      .transform([&] { return std::move(attribute); });
}

auto parse_attached_component_attributes(const YAML::Node& attached_component_node,
                                         const ir::Component& prototype)
    -> Result<Vector<ir::NamedAttribute>>
{
  Vector<ir::NamedAttribute> attributes {};

  if (const auto value_seq = attached_component_node["values"]) {
    attributes.reserve(value_seq.size());

    for (const auto& value_node : value_seq) {
      if (auto attribute = parse_attached_component_attribute(value_node, prototype)) {
        attributes.push_back(std::move(*attribute));
      }
      else {
        return propagate_unexpected(attribute);
      }
    }
  }

  return attributes;
}

auto parse_attached_component(const YAML::Node& attached_component_node,
                              const ir::Map& map) -> Result<ir::AttachedComponent>
{
  TACTILE_LOG_TRACE("Parsing attached component node at {}:{}",
                    attached_component_node.Mark().line,
                    attached_component_node.Mark().column);

  ir::AttachedComponent attached_component {};

  return parse_to(attached_component_node, "type", attached_component.type)
      .and_then([&]() -> Result<const ir::Component*> {
        const auto prototype_component_iter =
            std::ranges::find_if(map.components, [&](const ir::Component& component) {
              return component.name == attached_component.type;
            });

        if (prototype_component_iter == map.components.end()) {
          return unexpected(
              make_save_format_error(SaveFormatError::kInvalidComponentType));
        }

        return &*prototype_component_iter;
      })
      .and_then([&](const ir::Component* prototype) {
        return parse_attached_component_attributes(attached_component_node, *prototype);
      })
      .and_then([&](Vector<ir::NamedAttribute>&& attributes) {
        attached_component.attributes = std::move(attributes);
        return kOK;
      })
      .transform([&] { return std::move(attached_component); });
}

auto parse_attached_components(const YAML::Node& context_node, const ir::Map& map)
    -> Result<Vector<ir::AttachedComponent>>
{
  Vector<ir::AttachedComponent> attached_components {};

  const auto attached_component_seq = context_node["components"];
  if (!attached_component_seq) {
    return attached_components;
  }

  TACTILE_LOG_TRACE("Parsing attached component sequence at {}:{}",
                    attached_component_seq.Mark().line,
                    attached_component_seq.Mark().column);

  attached_components.reserve(attached_component_seq.size());

  for (const auto& attached_component_node : attached_component_seq) {
    if (auto attached_component =
            parse_attached_component(attached_component_node, map)) {
      attached_components.push_back(std::move(*attached_component));
    }
    else {
      return propagate_unexpected(attached_component);
    }
  }

  return attached_components;
}

auto parse_component_attribute(const YAML::Node& attribute_node)
    -> Result<ir::NamedAttribute>
{
  ir::NamedAttribute named_attribute {};

  return parse_to(attribute_node, "name", named_attribute.name)
      .and_then([&] { return parse<String>(attribute_node, "type"); })
      .and_then([&](const String& type_name) { return parse_property_type(type_name); })
      .and_then([&](const AttributeType attr_type) {
        named_attribute.value.reset(attr_type);
        return kOK;
      })
      .and_then([&] {
        if (const auto default_node = attribute_node["default"]) {
          if (auto default_value =
                  parse_property_value(default_node, named_attribute.value.get_type())) {
            named_attribute.value = std::move(*default_value);
          }
        }

        return kOK;
      })
      .transform([&] { return std::move(named_attribute); });
}

auto parse_component_attributes(const YAML::Node& component_node)
    -> Result<Vector<ir::NamedAttribute>>
{
  Vector<ir::NamedAttribute> attributes {};

  if (const auto attribute_seq = component_node["attributes"]) {
    attributes.reserve(attribute_seq.size());

    for (const auto& attribute_node : attribute_seq) {
      if (auto attribute = parse_component_attribute(attribute_node)) {
        attributes.push_back(std::move(*attribute));
      }
      else {
        return propagate_unexpected(attribute);
      }
    }
  }

  return attributes;
}

auto parse_component(const YAML::Node& component_node) -> Result<ir::Component>
{
  TACTILE_LOG_TRACE("Parsing component node at {}:{}",
                    component_node.Mark().line,
                    component_node.Mark().column);

  ir::Component component {};

  return parse_to(component_node, "name", component.name)
      .and_then([&] { return parse_component_attributes(component_node); })
      .and_then([&](Vector<ir::NamedAttribute>&& attributes) {
        component.attributes = std::move(attributes);
        return kOK;
      })
      .transform([&] { return std::move(component); });
}

auto parse_components(const YAML::Node& root_node) -> Result<Vector<ir::Component>>
{
  Vector<ir::Component> components {};

  if (const auto component_seq = root_node["component-definitions"]) {
    TACTILE_LOG_TRACE("Parsing component sequence at {}:{}",
                      component_seq.Mark().line,
                      component_seq.Mark().column);
    components.reserve(component_seq.size());

    for (const auto& component_node : component_seq) {
      if (auto component = parse_component(component_node)) {
        components.push_back(std::move(*component));
      }
      else {
        return propagate_unexpected(component);
      }
    }
  }

  return components;
}

}  // namespace tactile::yml_format
