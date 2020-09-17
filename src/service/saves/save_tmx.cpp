#include "save_tmx.hpp"

#include <qdir.h>
#include <qfileinfo.h>

#include <pugixml/pugixml.hpp>
#include <string>  // string

#include "tactile_error.hpp"
#include "to_string.hpp"

// TODO consider setting for inclusion of default values, e.g. opacity of layers

using namespace tactile::core;

namespace tactile::service {
namespace {

template <typename T, typename U>
void add_attribute(pugi::xml_node& node, T&& name, U&& value)
{
  node.append_attribute(name).set_value(value);
}

void save_tilesets(pugi::xml_node& root,
                   const tileset_manager& tilesets,
                   const QString& destination)
{
  for (const auto& [id, tileset] : tilesets) {
    auto tilesetNode = root.append_child("tileset");
    add_attribute(tilesetNode, "firstgid", tileset.first_id().get());

    const auto name = tileset.name().toStdString();
    add_attribute(tilesetNode, "name", name.c_str());

    /*
     * Reminder: Tiled tilesets can be external or "inline", if source is used,
     * then we need to create separate files for the tilesets.
     */

    add_attribute(tilesetNode, "tilewidth", tileset.get_tile_width().get());
    add_attribute(tilesetNode, "tileheight", tileset.get_tile_height().get());
    add_attribute(tilesetNode, "tilecount", tileset.tile_count());
    add_attribute(tilesetNode, "columns", tileset.cols().get());

    // this node should only be used for embedded tilesets
    auto imageNode = tilesetNode.append_child("image");

    const QFileInfo info{destination};
    const auto relativePath = info.dir().relativeFilePath(tileset.path());
    const auto imagePath = relativePath.toStdString();

    add_attribute(imageNode, "source", imagePath.c_str());
    add_attribute(imageNode, "width", tileset.width());
    add_attribute(imageNode, "height", tileset.height());
  }
}

void save_layers(pugi::xml_node& root, const map& map)
{
  std::string stringBuffer;
  const auto count = map.rows().get() * map.cols().get();
  stringBuffer.reserve(count * 2);

  for (int id{1}; const auto& layer : map) {
    auto layerNode = root.append_child("layer");

    add_attribute(layerNode, "id", id);

    if (const auto str = to_string(id); str) {
      const auto name = "Layer " + *str;
      add_attribute(layerNode, "name", name.c_str());
    }

    add_attribute(layerNode, "width", layer.cols().get());
    add_attribute(layerNode, "height", layer.rows().get());

    if (!layer.visible()) {
      add_attribute(layerNode, "visible", 0);
    }

    auto data = layerNode.append_child("data");
    add_attribute(data, "encoding", "csv");

    bool fst{true};
    layer.for_each([&](tile_id tile) {
      if (!fst) {
        using namespace std::string_literals;
        stringBuffer += ","s;
      } else {
        fst = false;
      }

      if (const auto str = to_string(tile.get()); str) {
        stringBuffer += *str;
      } else {
        throw tactile_error{"Failed to write tile data to string buffer!"};
      }
    });

    data.text().set(stringBuffer.c_str());
    stringBuffer.clear();
    ++id;
  }
}

[[nodiscard]] auto create_root(pugi::xml_document& document, const map& map)
    -> pugi::xml_node
{
  auto root = document.append_child("map");

  add_attribute(root, "version", "1.4");
  add_attribute(root, "orientation", "orthogonal");
  add_attribute(root, "renderorder", "right-down");
  add_attribute(root, "width", map.cols().get());
  add_attribute(root, "height", map.rows().get());

  // FIXME what are reasonable assumptions here, perhaps a setting for this?
  add_attribute(root, "tilewidth", 32);
  add_attribute(root, "tileheight", 32);

  return root;
}

}  // namespace

void save_tmx(const QString& path,
              const map& map,
              const tileset_manager& tilesets)
{
  pugi::xml_document document{};

  auto root = create_root(document, map);
  save_tilesets(root, tilesets, path);
  save_layers(root, map);

  const auto pathStr = path.toStdString();
  document.save_file(pathStr.c_str());
}

}  // namespace tactile::service
