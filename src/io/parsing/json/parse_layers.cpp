#include "parse_layers.hpp"

#include <string>   // string
#include <utility>  // move

#include "core/map/layers/tile_layer.hpp"
#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const nlohmann::json& json, LayerData& data)
    -> ParseError;

[[nodiscard]] auto ParseGroupLayer(const nlohmann::json& json, LayerData& target)
    -> ParseError
{
  auto& data = target.data.emplace<GroupLayerData>();

  if (const auto it = json.find("layers"); it != json.end())
  {
    for (const auto& [key, layer] : it->items())
    {
      auto layerData = std::make_unique<LayerData>();

      if (const auto err = ParseLayer(layer, *layerData); err != ParseError::None)
      {
        return err;
      }

      data.layers.push_back(std::move(layerData));
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const nlohmann::json& json, LayerData& data)
    -> ParseError
{
  if (const auto it = json.find("id"); it != json.end())
  {
    data.id = layer_id{it->get<layer_id::value_type>()};
  }
  else
  {
    return ParseError::LayerMissingId;
  }

  if (const auto it = json.find("name"); it != json.end())
  {
    data.name = it->get<std::string>();
  }
  else
  {
    data.name = "Layer";
  }

  if (const auto it = json.find("opacity"); it != json.end())
  {
    data.opacity = it->get<float>();
  }
  else
  {
    data.opacity = 1.0f;
  }

  if (const auto it = json.find("visible"); it != json.end())
  {
    data.is_visible = it->get<bool>();
  }
  else
  {
    data.is_visible = true;
  }

  if (const auto it = json.find("type"); it != json.end())
  {
    const auto type = it->get<std::string>();
    if (type == "tilelayer")
    {
      data.type = LayerType::TileLayer;
      if (const auto err = ParseTileLayer(json, data); err != ParseError::None)
      {
        return err;
      }
    }
    else if (type == "objectgroup")
    {
      data.type = LayerType::ObjectLayer;
      if (const auto err = ParseObjectLayer(json, data); err != ParseError::None)
      {
        return err;
      }
    }
    else if (type == "group")
    {
      data.type = LayerType::GroupLayer;
      if (const auto err = ParseGroupLayer(json, data); err != ParseError::None)
      {
        return err;
      }
    }
    else
    {
      return ParseError::LayerUnknownType;
    }
  }
  else
  {
    return ParseError::LayerMissingType;
  }

  if (const auto err = ParseProperties(json, data.properties);
      err != ParseError::None)
  {
    return err;
  }

  return ParseError::None;
}

}  // namespace

auto ParseLayers(const nlohmann::json& json, std::vector<LayerData>& layers)
    -> ParseError
{
  if (const auto it = json.find("layers"); it != json.end())
  {
    for (const auto& [key, layer] : it->items())
    {
      auto& layerData = layers.emplace_back();
      if (const auto err = ParseLayer(layer, layerData); err != ParseError::None)
      {
        return err;
      }
    }
  }
  else
  {
    return ParseError::MapMissingLayers;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
