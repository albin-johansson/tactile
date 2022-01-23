#include "save_object.hpp"

#include <cstring>  // strcmp

#include <yaml-cpp/yaml.h>

#include "save_components.hpp"
#include "save_properties.hpp"

namespace tactile::IO {

void SaveObject(YAML::Emitter& emitter,
                const Object& object,
                const std::filesystem::path& dir)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << GetId(object);

  emitter << YAML::Key << "type" << YAML::Value;
  switch (GetType(object)) {
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

  if (const auto* name = GetName(object); std::strcmp(name, "") != 0) {
    emitter << YAML::Key << "name" << YAML::Value << name;
  }

  if (const auto* tag = GetTag(object); std::strcmp(tag, "") != 0) {
    emitter << YAML::Key << "tag" << YAML::Value << tag;
  }

  if (const auto visible = IsVisible(object); !visible) {
    emitter << YAML::Key << "visible" << YAML::Value << visible;
  }

  if (const auto x = GetX(object); x != 0.0f) {
    emitter << YAML::Key << "x" << YAML::Value << x;
  }

  if (const auto y = GetY(object); y != 0.0f) {
    emitter << YAML::Key << "y" << YAML::Value << y;
  }

  if (const auto width = GetWidth(object); width != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << width;
  }

  if (const auto height = GetHeight(object); height != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << height;
  }

  SaveProperties(emitter, object, dir);
  SaveComponents(emitter, object, dir);

  emitter << YAML::EndMap;
}

}  // namespace tactile::IO
