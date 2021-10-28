#include "append_layer.hpp"

#include <sstream>  // stringstream
#include <variant>  // get

#include <tactile-base/tactile_error.hpp>

#include "append_object.hpp"
#include "append_properties.hpp"

namespace Tactile::IO {
namespace {

void AppendCommonLayerAttributes(pugi::xml_node node, const Layer& layer)
{
  node.append_attribute("id").set_value(GetId(layer));
  node.append_attribute("name").set_value(GetName(layer));

  if (const auto opacity = GetOpacity(layer); opacity != 1.0f) {
    node.append_attribute("opacity").set_value(opacity);
  }

  if (const auto visible = IsVisible(layer); !visible) {
    node.append_attribute("visible").set_value(0);
  }
}

void AppendTileLayer(pugi::xml_node mapNode,
                     const Layer& layer,
                     const std::filesystem::path& dir,
                     const bool humanReadableOutput)
{
  const auto& tileLayer = GetTileLayer(layer);

  auto node = mapNode.append_child("layer");
  AppendCommonLayerAttributes(node, layer);

  const auto nRows = GetRowCount(tileLayer);
  const auto nCols = GetColumnCount(tileLayer);
  node.append_attribute("width").set_value(nCols);
  node.append_attribute("height").set_value(nRows);

  AppendProperties(node, layer, dir);

  auto data = node.append_child("data");
  data.append_attribute("encoding").set_value("csv");

  const auto count = nRows * nCols;

  std::stringstream stream;
  usize index = 0;

  for (int32 row = 0; row < nRows; ++row) {
    for (int32 col = 0; col < nCols; ++col) {
      if (humanReadableOutput && index == 0) {
        stream << '\n';
      }

      stream << GetTile(tileLayer, row, col);
      if (index < static_cast<usize>(count - 1)) {
        stream << ',';
      }

      if (humanReadableOutput && (index + 1) % nCols == 0) {
        stream << '\n';
      }

      ++index;
    }
  }

  const auto tileData = stream.str();
  data.text().set(tileData.c_str());
}

void AppendObjectLayer(pugi::xml_node mapNode,
                       const Layer& layer,
                       const std::filesystem::path& dir)
{
  const auto& objectLayer = GetObjectLayer(layer);

  auto node = mapNode.append_child("objectgroup");
  AppendCommonLayerAttributes(node, layer);
  AppendProperties(node, layer, dir);

  EachObject(objectLayer, [&](const Object& object) { AppendObject(node, object, dir); });
}

void AppendGroupLayer(pugi::xml_node mapNode,
                      const Layer& layer,
                      const std::filesystem::path& dir,
                      const bool humanReadableOutput)
{
  const auto& groupLayer = GetGroupLayer(layer);

  auto node = mapNode.append_child("group");
  AppendCommonLayerAttributes(node, layer);

  EachLayer(groupLayer, [&](const Layer& child) {
    AppendLayer(node, child, dir, humanReadableOutput);
  });
}

}  // namespace

void AppendLayer(pugi::xml_node mapNode,
                 const Layer& layer,
                 const std::filesystem::path& dir,
                 const bool humanReadableOutput)
{
  switch (GetType(layer)) {
    case LayerType::TileLayer: {
      AppendTileLayer(mapNode, layer, dir, humanReadableOutput);
      break;
    }
    case LayerType::ObjectLayer: {
      AppendObjectLayer(mapNode, layer, dir);
      break;
    }
    case LayerType::GroupLayer: {
      AppendGroupLayer(mapNode, layer, dir, humanReadableOutput);
      break;
    }
    default:
      throw TactileError{"Did not recognize layer type when saving as XML!"};
  }
}

}  // namespace Tactile::IO
