#include "save_properties.hpp"

#include <yaml-cpp/yaml.h>

#include "../../common_saving.hpp"
#include "ir_helpers.hpp"

namespace Tactile::IO {
namespace {

template <typename T>
void SavePropertiesImpl(YAML::Emitter& emitter,
                        const T& source,
                        const std::filesystem::path& dir)
{
  if (GetPropertyCount(source) != 0) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    EachProperty(source, [&](const Property& property) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << GetName(property);

      const auto type = GetType(property);
      emitter << YAML::Key << "type" << YAML::Value << GetPropertyTypeString(type);

      emitter << YAML::Key << "value";
      switch (type) {
        case PropertyType::String:
          emitter << YAML::Value << GetString(property);
          break;

        case PropertyType::Integer:
          emitter << YAML::Value << GetInt(property);
          break;

        case PropertyType::Floating:
          emitter << YAML::Value << GetFloat(property);
          break;

        case PropertyType::Boolean:
          emitter << YAML::Value << GetBool(property);
          break;

        case PropertyType::File:
          emitter << YAML::Value << GetPropertyFileValue(property, dir);
          break;

        case PropertyType::Color: {
          emitter << YAML::Value << AsRGBA(GetColor(property));
          break;
        }

        case PropertyType::Object:
          emitter << YAML::Value << GetObject(property);
          break;
      }

      emitter << YAML::EndMap;
    });

    emitter << YAML::EndSeq;
  }
}

}  // namespace

void SaveProperties(YAML::Emitter& emitter,
                    const Map& map,
                    const std::filesystem::path& dir)
{
  SavePropertiesImpl(emitter, map, dir);
}

void SaveProperties(YAML::Emitter& emitter,
                    const Layer& layer,
                    const std::filesystem::path& dir)
{
  SavePropertiesImpl(emitter, layer, dir);
}

void SaveProperties(YAML::Emitter& emitter,
                    const Tileset& tileset,
                    const std::filesystem::path& dir)
{
  SavePropertiesImpl(emitter, tileset, dir);
}

void SaveProperties(YAML::Emitter& emitter,
                    const Tile& tile,
                    const std::filesystem::path& dir)
{
  SavePropertiesImpl(emitter, tile, dir);
}

void SaveProperties(YAML::Emitter& emitter,
                    const Object& object,
                    const std::filesystem::path& dir)
{
  SavePropertiesImpl(emitter, object, dir);
}

}  // namespace Tactile::IO
