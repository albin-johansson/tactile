#include "xml_map_object.hpp"

#include <algorithm>  // any_of
#include <cassert>    // assert
#include <cstring>    // strcmp
#include <string>     // string
#include <utility>    // move

#include "core/map/map_position.hpp"
#include "core/tactile_error.hpp"
#include "parse_csv.hpp"
#include "parse_tile_nodes.hpp"

namespace Tactile::IO {

XmlMapObject::XmlMapObject(Shared<pugi::xml_document> document,
                           const pugi::xml_node node)
    : mDocument{std::move(document)}
    , mNode{node}
{}

XmlMapObject::XmlMapObject(const std::filesystem::path& path)
    : mDocument{std::make_shared<pugi::xml_document>()}
{
  if (const auto result = mDocument->load_file(path.c_str()); !result)
  {
    throw TactileError{"Could not create XmlMapObject!"};
  }

  mNode = mDocument->root();
}

auto XmlMapObject::FromFile(const std::filesystem::path& path) const -> Object
{
  try
  {
    return std::make_unique<XmlMapObject>(path);
  }
  catch (...)
  {
    return nullptr;
  }
}

auto XmlMapObject::Contains(const MapAttribute attr) const -> bool
{
  const auto key = ToKey(attr);
  return std::any_of(mNode.attributes_begin(),
                     mNode.attributes_end(),
                     [=](const auto& attribute) { return attribute.name() == key; });
}

auto XmlMapObject::GetInt(const MapAttribute attr) const -> Maybe<int>
{
  const auto key = ToKey(attr);
  const auto& attribute = mNode.attribute(key.data());
  if (!attribute.empty())
  {
    return attribute.as_int();
  }
  else
  {
    return nothing;
  }
}

auto XmlMapObject::GetFloat(const MapAttribute attr) const -> Maybe<float>
{
  const auto key = ToKey(attr);
  const auto& attribute = mNode.attribute(key.data());
  if (!attribute.empty())
  {
    return attribute.as_float();
  }
  else
  {
    return nothing;
  }
}

auto XmlMapObject::GetBool(const MapAttribute attr) const -> Maybe<bool>
{
  const auto key = ToKey(attr);
  const auto& attribute = mNode.attribute(key.data());
  if (!attribute.empty())
  {
    return attribute.as_bool();
  }
  else
  {
    return nothing;
  }
}

auto XmlMapObject::GetString(const MapAttribute attr) const -> Maybe<std::string>
{
  const auto key = ToKey(attr);
  const auto& attribute = mNode.attribute(key.data());
  if (!attribute.empty())
  {
    if (const auto str = attribute.as_string(nullptr))
    {
      return std::string{str};
    }
  }

  return nothing;
}

auto XmlMapObject::GetArray(const MapAttribute attr) const -> Objects
{
  const auto key = ToKey(attr);
  return GetArray(key);
}

auto XmlMapObject::GetArray(const std::string_view key) const -> Objects
{
  Objects objects;

  const auto children = mNode.children(key.data());
  for (const auto& node : children)
  {
    objects.push_back(std::make_unique<XmlMapObject>(mDocument, node));
  }

  return objects;
}

auto XmlMapObject::GetTilesetImageRelativePath() const
    -> Maybe<std::filesystem::path>
{
  const auto tilesetNode = std::strcmp(mNode.name(), "tileset") == 0
                               ? mNode
                               : mNode.find_node([](const pugi::xml_node& node) {
                                   return std::strcmp(node.name(), "tileset") == 0;
                                 });

  const auto imageNode = tilesetNode.child("image");
  const auto source = imageNode.attribute("source");

  if (const auto* str = source.as_string(nullptr))
  {
    return std::filesystem::path{str};
  }
  else
  {
    return nothing;
  }
}

auto XmlMapObject::IsTileLayer() const -> bool
{
  return std::strcmp(mNode.name(), "layer") == 0;
}

auto XmlMapObject::IsObjectLayer() const -> bool
{
  return std::strcmp(mNode.name(), "objectgroup") == 0;
}

auto XmlMapObject::IsImplicitStringProperty() const -> bool
{
  return std::strcmp(mNode.name(), "property") == 0 &&
         std::strcmp(mNode.attribute("type").name(), "") == 0;
}

auto XmlMapObject::IsPoint() const -> bool
{
  return !mNode.child("point").empty();
}

auto XmlMapObject::IsEllipse() const -> bool
{
  return !mNode.child("ellipse").empty();
}

auto XmlMapObject::HasType() const -> bool
{
  return true;
}

auto XmlMapObject::GetTileData(const col_t nCols, TileMatrix& matrix) const
    -> ParseError
{
  assert(IsTileLayer());

  const auto data = mNode.child("data");

  /* The encoding attribute is optional, if it is missing then the tile data is
     stored as individual "tile" nodes. */
  if (const auto* encoding = data.attribute("encoding").as_string(nullptr))
  {
    // We only support the CSV encoding (when explicit)
    if (std::strcmp(encoding, "csv") != 0)
    {
      return ParseError::UnsupportedTileEncoding;
    }
    else
    {
      const auto text = data.text();
      if (const auto error = ParseCSV(text.get(), nCols, matrix);
          error != ParseError::None)
      {
        return error;
      }
    }
  }
  else
  {
    if (const auto error = ParseTileNodes(data, nCols, matrix);
        error != ParseError::None)
    {
      return error;
    }
  }

  return ParseError::None;
}

auto XmlMapObject::GetObjects() const -> Objects
{
  assert(IsObjectLayer());
  return GetArray("object");
}

auto XmlMapObject::GetProperties() const -> Objects
{
  Objects objects;

  for (const auto& node : mNode.child("properties").children("property"))
  {
    objects.push_back(std::make_unique<XmlMapObject>(mDocument, node));
  }

  return objects;
}

auto XmlMapObject::GetExternalTileset() const -> Object
{
  return std::make_unique<XmlMapObject>(mDocument, mNode.child("tileset"));
}

auto XmlMapObject::ToKey(const MapAttribute attr) -> std::string_view
{
  switch (attr)
  {
    case MapAttribute::Tileset:
      return "tileset";

    case MapAttribute::NextLayerId:
      return "nextlayerid";

    case MapAttribute::NextObjectId:
      return "nextobjectid";

    case MapAttribute::Source:
      return "source";

    case MapAttribute::FirstGid:
      return "firstgid";

    case MapAttribute::TileWidth:
      return "tilewidth";

    case MapAttribute::TileHeight:
      return "tileheight";

    case MapAttribute::Image:
      return "image";

    case MapAttribute::Name:
      return "name";

    case MapAttribute::Id:
      return "id";

    case MapAttribute::Width:
      return "width";

    case MapAttribute::Height:
      return "height";

    case MapAttribute::Visible:
      return "visible";

    case MapAttribute::Opacity:
      return "opacity";

    case MapAttribute::Type:
      return "type";

    case MapAttribute::Value:
      return "value";

    case MapAttribute::X:
      return "x";

    case MapAttribute::Y:
      return "y";

    case MapAttribute::Point:
      return "point";

    case MapAttribute::Orientation:
      return "orientation";

    case MapAttribute::Infinite:
      return "infinite";

    default:
      throw TactileError{"Did not recognize attribute!"};
  }
}

}  // namespace Tactile::IO
