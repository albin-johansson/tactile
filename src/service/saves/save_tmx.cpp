#include "save_tmx.hpp"

#include <qdir.h>
#include <qfileinfo.h>

#include <pugixml/pugixml.hpp>
#include <string>  // string

#include "export_options.hpp"
#include "preferences.hpp"
#include "tactile_error.hpp"
#include "tiled_version.hpp"
#include "to_string.hpp"

// TODO adhere to prefs::saves::generate_defaults, prefs::saves::embed_tilesets

using namespace tactile::core;

namespace tactile::service {
namespace {

template <typename T, typename U>
void add_attribute(pugi::xml_node& node, T&& name, U&& value)
{
  node.append_attribute(name).set_value(value);
}

void add_common_attributes(pugi::xml_node& node,
                           const tileset& tileset,
                           const QString& mapDestination,
                           const export_options& options)
{
  const auto name = tileset.name().toStdString();
  add_attribute(node, "version", g_tiledXmlVersion);
  add_attribute(node, "tiledversion", g_tiledVersion);
  add_attribute(node, "name", name.c_str());
  add_attribute(node, "tilewidth", tileset.get_tile_width().get());
  add_attribute(node, "tileheight", tileset.get_tile_height().get());
  add_attribute(node, "tilecount", tileset.tile_count());
  add_attribute(node, "columns", tileset.cols().get());
}

void add_image_node(pugi::xml_node& parent,
                    const tileset& tileset,
                    const QString& mapDestination)
{
  auto node = parent.append_child("image");

  const QFileInfo info{mapDestination};
  const auto relativePath = info.dir().relativeFilePath(tileset.path());
  const auto imagePath = relativePath.toStdString();

  add_attribute(node, "source", imagePath.c_str());
  add_attribute(node, "width", tileset.width());
  add_attribute(node, "height", tileset.height());
}

void create_external_tileset_file(const tileset& tileset,
                                  const QString& mapDestination,
                                  const export_options& options)
{
  pugi::xml_document document{};

  auto node = document.append_child("tileset");
  add_common_attributes(node, tileset, mapDestination, options);
  add_image_node(node, tileset, mapDestination);

  const QFileInfo info{mapDestination};
  const auto path = info.absoluteDir().absoluteFilePath(
      tileset.name() + QStringLiteral(u".tsx"));
  document.save_file(path.toStdString().c_str());
}

void save_tilesets(pugi::xml_node& root,
                   const tileset_manager& tilesets,
                   const QString& mapDestination,
                   const export_options& options)
{
  for (const auto& [id, tileset] : tilesets) {
    auto tilesetNode = root.append_child("tileset");

    add_attribute(tilesetNode, "firstgid", tileset.first_id().get());

    if (options.embedTilesets) {
      add_common_attributes(tilesetNode, tileset, mapDestination, options);
      add_image_node(tilesetNode, tileset, mapDestination);
    } else {
      const QFileInfo info{mapDestination};
      const auto source =
          info.dir().relativeFilePath(tileset.name() + QStringLiteral(u".tsx"));
      add_attribute(tilesetNode, "source", source.toStdString().c_str());
      create_external_tileset_file(tileset, mapDestination, options);
    }
  }
}

void save_layers(pugi::xml_node& root, const map& map)
{
  std::string stringBuffer;
  const auto count = map.rows().get() * map.cols().get();
  stringBuffer.reserve(count * 2);

  int id{1};
  for (const auto& layer : map) {
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

[[nodiscard]] auto create_root(pugi::xml_document& document,
                               const map& map,
                               const export_options& options) -> pugi::xml_node
{
  auto root = document.append_child("map");

  add_attribute(root, "version", g_tiledXmlVersion);
  add_attribute(root, "tiledversion", g_tiledVersion);
  add_attribute(root, "orientation", "orthogonal");
  add_attribute(root, "renderorder", "right-down");
  add_attribute(root, "width", map.cols().get());
  add_attribute(root, "height", map.rows().get());
  add_attribute(root, "tilewidth", prefs::saves::tile_width().value());
  add_attribute(root, "tileheight", prefs::saves::tile_height().value());
  add_attribute(root, "infinite", 0);
  add_attribute(root, "nextlayerid", map.num_layers() + 1);
  add_attribute(root, "nextobjectid", 1);

  if (options.generateDefaults) {
    add_attribute(root, "compressionlevel", -1);
    add_attribute(root, "backgroundcolor", "#00000000");
  }

  return root;
}

}  // namespace

void save_tmx(const QString& path,
              const map& map,
              const tileset_manager& tilesets)
{
  const auto options = make_export_options();

  pugi::xml_document document{};

  auto root = create_root(document, map, options);
  save_tilesets(root, tilesets, path, options);
  save_layers(root, map);

  const auto pathStr = path.toStdString();
  document.save_file(pathStr.c_str());
}

}  // namespace tactile::service
