#include "save_components.hpp"

#include <yaml-cpp/yaml.h>

#include "common_saving.hpp"

namespace tactile::IO {
namespace {

void SaveColor() {}

template <typename T>
void SaveComponentsImpl(YAML::Emitter& emitter,
                        const T& source,
                        const std::filesystem::path& dir)
{
  if (GetComponentCount(source) != 0) {
    emitter << YAML::Key << "components" << YAML::BeginSeq;

    EachComponent(source, [&](const Component& component) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "type" << YAML::Value << GetName(component);
      emitter << YAML::Key << "values" << YAML::BeginSeq;

      EachAttribute(component, [&](const CStr attr) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name" << YAML::Value << attr;

        emitter << YAML::Key << "value";
        switch (GetAttributeType(component, attr)) {
          case PropertyType::String:
            emitter << YAML::Value << GetString(component, attr);
            break;

          case PropertyType::Integer:
            emitter << YAML::Value << GetInt(component, attr);
            break;

          case PropertyType::Floating:
            emitter << YAML::Value << GetFloat(component, attr);
            break;

          case PropertyType::Boolean:
            emitter << YAML::Value << GetBool(component, attr);
            break;

          case PropertyType::File:
            emitter << YAML::Value << GetFile(component, attr);
            break;

          case PropertyType::Color:
            emitter << YAML::Value << AsRGBA(GetColor(component, attr));
            break;

          case PropertyType::Object:
            emitter << YAML::Value << GetObject(component, attr);
            break;
        }

        emitter << YAML::EndMap;
      });

      emitter << YAML::EndSeq;
      emitter << YAML::EndMap;
    });

    emitter << YAML::EndSeq;
  }
}

}  // namespace

void SaveComponentDefinitions(YAML::Emitter& emitter,
                              const Map& map,
                              const std::filesystem::path& dir)
{
  if (GetComponentDefCount(map) != 0) {
    emitter << YAML::Key << "component-definitions" << YAML::BeginSeq;

    EachComponentDef(map, [&](const ComponentDef& def) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << GetName(def);

      if (GetAttributeCount(def) != 0) {
        emitter << YAML::Key << "attributes" << YAML::BeginSeq;

        EachAttribute(def, [&](const CStr attr) {
          emitter << YAML::BeginMap;
          emitter << YAML::Key << "name" << YAML::Value << attr;

          emitter << YAML::Key << "type";
          const auto type = GetAttributeType(def, attr);
          switch (type) {
            case PropertyType::String:
              emitter << YAML::Value << "string";
              break;

            case PropertyType::Integer:
              emitter << YAML::Value << "int";
              break;

            case PropertyType::Floating:
              emitter << YAML::Value << "float";
              break;

            case PropertyType::Boolean:
              emitter << YAML::Value << "bool";
              break;

            case PropertyType::File:
              emitter << YAML::Value << "file";
              break;

            case PropertyType::Color:
              emitter << YAML::Value << "color";
              break;

            case PropertyType::Object:
              emitter << YAML::Value << "object";
              break;
          }

          if (!HasDefaultValue(def, attr)) {
            emitter << YAML::Key << "default";

            switch (type) {
              case PropertyType::String:
                emitter << YAML::Value << GetString(def, attr);
                break;

              case PropertyType::Integer:
                emitter << YAML::Value << GetInt(def, attr);
                break;

              case PropertyType::Floating:
                emitter << YAML::Value << GetFloat(def, attr);
                break;

              case PropertyType::Boolean:
                emitter << YAML::Value << GetBool(def, attr);
                break;

              case PropertyType::File:
                emitter << YAML::Value << GetFile(def, attr);
                break;

              case PropertyType::Color:
                emitter << YAML::Value << AsRGBA(GetColor(def, attr));
                break;

              case PropertyType::Object:
                emitter << YAML::Value << GetObject(def, attr);
                break;
            }
          }

          emitter << YAML::EndMap;
        });

        emitter << YAML::EndSeq;
      }

      emitter << YAML::EndMap;
    });

    emitter << YAML::EndSeq;
  }
}

void SaveComponents(YAML::Emitter& emitter,
                    const Map& map,
                    const std::filesystem::path& dir)
{
  SaveComponentsImpl(emitter, map, dir);
}

void SaveComponents(YAML::Emitter& emitter,
                    const Layer& layer,
                    const std::filesystem::path& dir)
{
  SaveComponentsImpl(emitter, layer, dir);
}

void SaveComponents(YAML::Emitter& emitter,
                    const Object& object,
                    const std::filesystem::path& dir)
{
  SaveComponentsImpl(emitter, object, dir);
}

void SaveComponents(YAML::Emitter& emitter,
                    const Tileset& tileset,
                    const std::filesystem::path& dir)
{
  SaveComponentsImpl(emitter, tileset, dir);
}

void SaveComponents(YAML::Emitter& emitter,
                    const Tile& tile,
                    const std::filesystem::path& dir)
{
  SaveComponentsImpl(emitter, tile, dir);
}

}  // namespace tactile::IO
