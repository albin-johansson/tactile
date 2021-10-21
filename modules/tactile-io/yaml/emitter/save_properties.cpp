#include "save_properties.hpp"

#include <magic_enum.hpp>  // enum_name
#include <yaml-cpp/yaml.h>

#include "../../common_saving.hpp"

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

      const auto type = property.value.GetType().value();
      emitter << YAML::Key << "type" << YAML::Value << magic_enum::enum_name(type).data();

      emitter << YAML::Key << "value";
      switch (type) {
        case PropertyType::String:
          emitter << YAML::Value << property.value.AsString();
          break;

        case PropertyType::Integer:
          emitter << YAML::Value << property.value.AsInt();
          break;

        case PropertyType::Floating:
          emitter << YAML::Value << property.value.AsFloat();
          break;

        case PropertyType::Boolean:
          emitter << YAML::Value << property.value.AsBool();
          break;

        case PropertyType::File:
          emitter << YAML::Value << GetPropertyFileValue(property.value, dir);
          break;

        case PropertyType::Color:
          emitter << YAML::Value << property.value.AsColor().as_rgba();
          break;

        case PropertyType::Object:
          emitter << YAML::Value << property.value.AsObject().get();
          break;
      }

      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
