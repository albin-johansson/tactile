#include "save_json.hpp"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include "preferences.hpp"
#include "to_string.hpp"

// TODO adhere to prefs::saves::generate_defaults, prefs::saves::embed_tilesets

using namespace tactile::core;

namespace tactile::service {
namespace {

[[nodiscard]] auto save_tilesets(const tileset_manager& tilesets,
                                 const QString& destination) -> QJsonArray
{
  QJsonArray array;

  for (const auto& [id, tileset] : tilesets) {
    QJsonObject object;

    object.insert(u"firstgid", tileset.first_id().get());
    object.insert(u"name", tileset.name());
    object.insert(u"tilewidth", tileset.get_tile_width().get());
    object.insert(u"tileheight", tileset.get_tile_height().get());
    object.insert(u"tilecount", tileset.tile_count());
    object.insert(u"columns", tileset.cols().get());
    object.insert(u"imagewidth", tileset.width());
    object.insert(u"imageheight", tileset.height());
    object.insert(u"margin", 0);
    object.insert(u"spacing", 0);

    const QFileInfo info{destination};
    object.insert(u"image", info.dir().relativeFilePath(tileset.path()));

    array.append(object);
  }

  return array;
}

[[nodiscard]] auto save_layers(const map& map) -> QJsonArray
{
  QJsonArray array;

  for (int id{1}; const auto& layer : map) {
    QJsonObject object;

    object.insert(u"compression", QStringLiteral(u""));
    object.insert(u"encoding", QStringLiteral(u"csv"));
    object.insert(u"width", layer.cols().get());
    object.insert(u"height", layer.rows().get());
    object.insert(u"id", id);

    if (const auto str = to_string(id); str) {
      const auto name = "Layer " + *str;
      object.insert(u"name", QString::fromStdString(name));
    }

    object.insert(u"offsetx", 0.0);
    object.insert(u"offsety", 0.0);
    object.insert(u"opacity", 1.0);
    object.insert(u"properties", QJsonArray{});
    object.insert(u"type", QStringLiteral(u"tilelayer"));
    object.insert(u"visible", layer.visible());
    object.insert(u"x", 0);
    object.insert(u"y", 0);

    QJsonArray data;
    layer.for_each([&](tile_id tile) { data.append(tile.get()); });
    object.insert(u"data", data);

    array.append(object);
    ++id;
  }

  return array;
}

[[nodiscard]] auto create_root(const map& map,
                               const tileset_manager& tilesets,
                               const QString& path) -> QJsonObject
{
  QJsonObject root;

  root.insert(u"tiledversion", QStringLiteral(u"1.4.2"));
  root.insert(u"orientation", QStringLiteral(u"orthogonal"));
  root.insert(u"renderorder", QStringLiteral(u"right-down"));
  root.insert(u"width", map.cols().get());
  root.insert(u"height", map.rows().get());
  root.insert(u"compressionlevel", -1);
  root.insert(u"infinite", false);
  root.insert(u"type", QStringLiteral(u"map"));
  root.insert(u"version", 1.4);
  root.insert(u"tilewidth", prefs::saves::tile_width().value());
  root.insert(u"tileheight", prefs::saves::tile_height().value());
  root.insert(u"tilesets", save_tilesets(tilesets, path));
  root.insert(u"layers", save_layers(map));

  return root;
}

}  // namespace

void save_json(const QString& path,
               const map& map,
               const tileset_manager& tilesets)
{
  QJsonDocument document{};
  document.setObject(create_root(map, tilesets, path));

  QFile file{path};
  file.open(QFile::WriteOnly);
  file.write(document.toJson());
}

}  // namespace tactile::service
