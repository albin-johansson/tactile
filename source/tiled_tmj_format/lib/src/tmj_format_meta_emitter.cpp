// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_meta_emitter.hpp"

#include <cstddef>  // size_t
#include <format>   // format

#include "tactile/base/document/meta_view.hpp"
#include "tactile/base/meta/attribute.hpp"

namespace tactile {

auto emit_tiled_tmj_property(const IMetaView& meta,
                             const std::size_t property_index) -> nlohmann::json
{
  const auto& [property_name, property_value] = meta.get_property(property_index);

  auto property_json = nlohmann::json::object();
  property_json["name"] = property_name;

  const auto property_type = property_value.get_type();
  switch (property_type) {
    case AttributeType::kStr: {
      property_json["type"] = "string";
      property_json["value"] = property_value.as_string();
      break;
    }
    case AttributeType::kInt: {
      property_json["type"] = "int";
      property_json["value"] = property_value.as_int();
      break;
    }
    case AttributeType::kInt2: {
      const auto& i2 = property_value.as_int2();

      property_json["type"] = "string";
      property_json["value"] = std::format("{};{}", i2.x(), i2.y());

      break;
    }
    case AttributeType::kInt3: {
      const auto& i3 = property_value.as_int3();

      property_json["type"] = "string";
      property_json["value"] = std::format("{};{};{}", i3.x(), i3.y(), i3.z());

      break;
    }
    case AttributeType::kInt4: {
      const auto& i4 = property_value.as_int4();

      property_json["type"] = "string";
      property_json["value"] = std::format("{};{};{};{}", i4.x(), i4.y(), i4.z(), i4.w());

      break;
    }
    case AttributeType::kFloat: {
      property_json["type"] = "float";
      property_json["value"] = property_value.as_float();
      break;
    }
    case AttributeType::kFloat2: {
      const auto& f2 = property_value.as_float2();

      property_json["type"] = "string";
      property_json["value"] = std::format("{};{}", f2.x(), f2.y());

      break;
    }
    case AttributeType::kFloat3: {
      const auto& f3 = property_value.as_float3();

      property_json["type"] = "string";
      property_json["value"] = std::format("{};{};{}", f3.x(), f3.y(), f3.z());

      break;
    }
    case AttributeType::kFloat4: {
      const auto& f4 = property_value.as_float4();

      property_json["type"] = "string";
      property_json["value"] = std::format("{};{};{};{}", f4.x(), f4.y(), f4.z(), f4.w());

      break;
    }
    case AttributeType::kBool: {
      property_json["type"] = "bool";
      property_json["value"] = property_value.as_bool();
      break;
    }
    case AttributeType::kPath: {
      property_json["type"] = "file";
      property_json["value"] = property_value.as_path().string();
      break;
    }
    case AttributeType::kColor: {
      const auto& color = property_value.as_color();

      property_json["type"] = "color";
      property_json["value"] = std::format("#{:02X}{:02X}{:02X}{:02X}",
                                           +color.alpha,
                                           +color.red,
                                           +color.green,
                                           +color.blue);

      break;
    }
    case AttributeType::kObject: {
      property_json["type"] = "object";
      property_json["value"] = property_value.as_object().value;
      break;
    }
  }

  return property_json;
}

void emit_tiled_tmj_metadata(const IMetaView& meta, nlohmann::json& meta_json)
{
  meta_json["name"] = meta.get_name();

  const auto property_count = meta.property_count();
  if (property_count == 0) {
    return;
  }

  auto property_array = nlohmann::json::array();
  property_array.get_ref<nlohmann::json::array_t&>().reserve(property_count);

  for (std::size_t index = 0; index < property_count; ++index) {
    property_array.push_back(emit_tiled_tmj_property(meta, index));
  }

  meta_json["properties"] = std::move(property_array);
}

}  // namespace tactile
