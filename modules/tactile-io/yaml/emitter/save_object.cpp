#include "save_object.hpp"

#include <yaml-cpp/yaml.h>

#include "save_properties.hpp"

namespace Tactile::IO {

void SaveObject(YAML::Emitter& emitter,
                const ObjectData& object,
                const std::filesystem::path& dir)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << object.id;

  emitter << YAML::Key << "type" << YAML::Value;
  switch (object.type) {
    case ObjectType::Point:
      emitter << "point";
      break;

    case ObjectType::Rectangle:
      emitter << "rect";
      break;

    case ObjectType::Ellipse:
      emitter << "ellipse";
      break;
  }

  if (!object.name.empty()) {
    emitter << YAML::Key << "name" << YAML::Value << object.name;
  }

  if (!object.custom_type.empty()) {
    emitter << YAML::Key << "tag" << YAML::Value << object.custom_type;
  }

  if (!object.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << object.visible;
  }

  if (object.x != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << object.x;
  }

  if (object.y != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << object.y;
  }

  if (object.width != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << object.width;
  }

  if (object.height != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << object.height;
  }

  SaveProperties(emitter, object.properties, dir);

  emitter << YAML::EndMap;
}

}  // namespace Tactile::IO
