// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/component_emitter.hpp"

#include "tactile/tactile-yml-format/emit/stream.hpp"

namespace tactile::yml_format {
namespace {

void _emit_component_attribute(YAML::Emitter& emitter,
                               const String& name,
                               const Attribute& value)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << name;
  emitter << YAML::Key << "type" << YAML::Value << value.get_type();

  if (!value.has_default_value()) {
    emitter << YAML::Key << "default" << YAML::Value << value;
  }

  emitter << YAML::EndMap;
}

}  // namespace

void emit_component(YAML::Emitter& emitter, const ir::Component& component)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << component.name;

  if (!component.attributes.empty()) {
    emitter << YAML::Key << "attributes" << YAML::BeginSeq;

    for (const auto& [attr_name, attr_value] : component.attributes) {
      _emit_component_attribute(emitter, attr_name, attr_value);
    }

    emitter << YAML::EndSeq;
  }

  emitter << YAML::EndMap;
}

void emit_component_seq(YAML::Emitter& emitter, const Vector<ir::Component>& components)
{
  if (components.empty()) {
    return;
  }

  emitter << YAML::Key << "component-definitions" << YAML::BeginSeq;

  for (const auto& component : components) {
    emit_component(emitter, component);
  }

  emitter << YAML::EndSeq;
}

}  // namespace tactile::yml_format
