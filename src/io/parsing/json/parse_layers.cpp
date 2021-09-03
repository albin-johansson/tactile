#include "parse_layers.hpp"

#include <string>   // string
#include <utility>  // move

#include "aliases/ints.hpp"
#include "parse_object_layer.hpp"
#include "parse_properties.hpp"
#include "parse_tile_layer.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto ParseLayer(const JSON& json, LayerData& data, usize index)
    -> ParseError;

[[nodiscard]] auto ParseGroupLayer(const JSON& json,
                                   LayerData& target,
                                   const usize index) -> ParseError
{
  auto& data = target.data.emplace<GroupLayerData>();

  if (const auto it = json.find("layers"); it != json.end())
  {
    usize childIndex = 0;
    for (const auto& [key, layer] : it->items())
    {
      auto layerData = std::make_unique<LayerData>();

      if (const auto err = ParseLayer(layer, *layerData, childIndex);
          err != ParseError::None)
      {
        return err;
      }

      data.layers.push_back(std::move(layerData));
      ++childIndex;
    }
  }

  return ParseError::None;
}

[[nodiscard]] auto ParseLayer(const JSON& json, LayerData& data, const usize index)
    -> ParseError
{
  data.index = index;

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
      if (const auto err = ParseGroupLayer(json, data, index);
          err != ParseError::None)
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

auto ParseLayers(const JSON& json, std::vector<LayerData>& layers) -> ParseError
{
  if (const auto it = json.find("layers"); it != json.end())
  {
    usize index = 0;
    for (const auto& [key, layer] : it->items())
    {
      auto& layerData = layers.emplace_back();
      if (const auto err = ParseLayer(layer, layerData, index);
          err != ParseError::None)
      {
        return err;
      }

      ++index;
    }
  }
  else
  {
    return ParseError::MapMissingLayers;
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
