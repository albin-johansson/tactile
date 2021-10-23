#include "save_properties.hpp"

#include <span>  // span

#include <magic_enum.hpp>  // enum_name
#include <yaml-cpp/yaml.h>

#include "../../common_saving.hpp"

namespace Tactile::IO {
namespace {

template <typename T>
void SavePropertiesImpl(YAML::Emitter& emitter,
                        const T& source,
                        const std::filesystem::path& dir)
{
  const auto count = GetPropertyCount(source);
  if (count != 0) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    for (usize index = 0; index < count; ++index) {
      const auto& property = GetProperty(source, index);

      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << GetName(property);

      const auto type = GetType(property);
      emitter << YAML::Key << "type" << YAML::Value << magic_enum::enum_name(type).data();

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
          const auto color = GetColor(property);
          const auto rgba =
              cen::color{color.red, color.green, color.blue, color.alpha}.as_rgba();
          emitter << YAML::Value << rgba;
          break;
        }

        case PropertyType::Object:
          emitter << YAML::Value << GetObject(property);
          break;
      }

      emitter << YAML::EndMap;
    }

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
