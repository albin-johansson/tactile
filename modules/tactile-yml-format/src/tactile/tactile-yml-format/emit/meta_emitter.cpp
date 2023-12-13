// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/meta_emitter.hpp"

#include <utility>  // to_underlying

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/save/vector_serialization.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/emit/stream.hpp"

namespace tactile::yml_format {
namespace {

void _emit_properties(YAML::Emitter& emitter, const ir::Metadata& metadata)
{
  if (!metadata.properties.empty()) {
    emitter << YAML::Key << "properties" << YAML::BeginSeq;

    for (const auto& [name, value] : metadata.properties) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "name" << YAML::Value << name;
      emitter << YAML::Key << "type" << YAML::Value << value.get_type();
      emitter << YAML::Key << "value" << YAML::Value << value;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

void _emit_components(YAML::Emitter& emitter, const ir::Metadata& metadata)
{
  if (!metadata.components.empty()) {
    emitter << YAML::Key << "components" << YAML::BeginSeq;

    for (const auto& [name, attributes] : metadata.components) {
      emitter << YAML::BeginMap;

      emitter << YAML::Key << "type" << YAML::Value << name;
      emitter << YAML::Key << "values" << YAML::BeginSeq;

      for (const auto& [attr_name, attr_value] : attributes) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name" << YAML::Value << attr_name;
        emitter << YAML::Key << "value" << YAML::Value << attr_value;
        emitter << YAML::EndMap;
      }

      emitter << YAML::EndSeq;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace

void emit(YAML::Emitter& emitter, const AttributeType type)
{
  switch (type) {
    case AttributeType::kStr: emitter << "string"; break;
    case AttributeType::kInt: emitter << "int"; break;
    case AttributeType::kInt2: emitter << "int2"; break;
    case AttributeType::kInt3: emitter << "int3"; break;
    case AttributeType::kInt4: emitter << "int4"; break;
    case AttributeType::kFloat: emitter << "float"; break;
    case AttributeType::kFloat2: emitter << "float2"; break;
    case AttributeType::kFloat3: emitter << "float3"; break;
    case AttributeType::kFloat4: emitter << "float4"; break;
    case AttributeType::kBool: emitter << "bool"; break;
    case AttributeType::kPath: emitter << "path"; break;
    case AttributeType::kColor: emitter << "color"; break;
    case AttributeType::kObject: emitter << "object"; break;
    default:
      TACTILE_LOG_ERROR("Did not recognize attribute type: {}", std::to_underlying(type));
      throw Exception {"Did not recognize attribute type"};
  }
}

void emit(YAML::Emitter& emitter, const Attribute& value)
{
  switch (value.get_type()) {
    case AttributeType::kStr: emitter << value.as_string(); break;
    case AttributeType::kInt: emitter << value.as_int(); break;
    case AttributeType::kInt2: emitter << serialize(value.as_int2()); break;
    case AttributeType::kInt3: emitter << serialize(value.as_int3()); break;
    case AttributeType::kInt4: emitter << serialize(value.as_int4()); break;
    case AttributeType::kFloat: emitter << value.as_float(); break;
    case AttributeType::kFloat2: emitter << serialize(value.as_float2()); break;
    case AttributeType::kFloat3: emitter << serialize(value.as_float3()); break;
    case AttributeType::kFloat4: emitter << serialize(value.as_float4()); break;
    case AttributeType::kBool: emitter << value.as_bool(); break;
    case AttributeType::kPath: emitter << normalize_path(value.as_path()); break;
    case AttributeType::kColor: emitter << to_string_rgba(value.as_color()); break;
    case AttributeType::kObject: emitter << value.as_object().value; break;
    default:
      TACTILE_LOG_ERROR("Did not recognize attribute type: {}",
                        std::to_underlying(value.get_type()));
      throw Exception {"Did not recognize attribute type"};
  }
}

void emit_metadata(YAML::Emitter& emitter, const ir::Metadata& metadata)
{
  _emit_properties(emitter, metadata);
  _emit_components(emitter, metadata);
}

}  // namespace tactile::yml_format
