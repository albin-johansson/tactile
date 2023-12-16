// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/object_emitter.hpp"

#include "tactile/tactile-yml-format/emit/meta_emitter.hpp"

namespace tactile::yml_format {

void emit_object(YAML::Emitter& emitter, const ir::Object& object)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "id" << YAML::Value << object.id.value;

  emitter << YAML::Key << "type";
  switch (object.type) {
    case ObjectType::kPoint: emitter << YAML::Value << "point"; break;
    case ObjectType::kRect: emitter << YAML::Value << "rect"; break;
    case ObjectType::kEllipse: emitter << YAML::Value << "ellipse"; break;
  }

  if (!object.name.empty()) {
    emitter << YAML::Key << "name" << YAML::Value << object.name;
  }

  if (!object.tag.empty()) {
    emitter << YAML::Key << "tag" << YAML::Value << object.tag;
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

  if (object.x != 0.0f) {
    emitter << YAML::Key << "width" << YAML::Value << object.width;
  }

  if (object.y != 0.0f) {
    emitter << YAML::Key << "height" << YAML::Value << object.height;
  }

  emit_metadata(emitter, object.meta);

  emitter << YAML::EndMap;
}

void emit_object_seq(YAML::Emitter& emitter, const Vector<ir::Object>& objects)
{
  if (objects.empty()) {
    return;
  }

  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  for (const auto& object : objects) {
    emit_object(emitter, object);
  }

  emitter << YAML::EndSeq;
}

}  // namespace tactile::yml_format
