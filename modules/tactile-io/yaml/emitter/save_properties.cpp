#include "save_properties.hpp"

#include <magic_enum.hpp>  // enum_name

#include "../../common_saving.hpp"

#include <yaml-cpp/yaml.h>

namespace Tactile::IO {

void SaveProperties(YAML::Emitter& emitter,
                    const std::vector<PropertyData>& properties,
                    const std::filesystem::path& dir)
{
  if (!properties.empty()) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    for (const auto& property : properties) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << property.name;

      const auto type = property.property.GetType().value();
      emitter << YAML::Key << "type" << YAML::Value << magic_enum::enum_name(type).data();

      emitter << YAML::Key << "value";
      switch (type) {
        case PropertyType::String:
          emitter << YAML::Value << property.property.AsString();
          break;

        case PropertyType::Integer:
          emitter << YAML::Value << property.property.AsInt();
          break;

        case PropertyType::Floating:
          emitter << YAML::Value << property.property.AsFloat();
          break;

        case PropertyType::Boolean:
          emitter << YAML::Value << property.property.AsBool();
          break;

        case PropertyType::File:
          emitter << YAML::Value << GetPropertyFileValue(property.property, dir);
          break;

        case PropertyType::Color:
          emitter << YAML::Value << property.property.AsColor().as_rgba();
          break;

        case PropertyType::Object:
          emitter << YAML::Value << property.property.AsObject().get();
          break;
      }

      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
