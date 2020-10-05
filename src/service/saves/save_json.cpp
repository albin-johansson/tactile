#include "save_json.hpp"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include "export_options.hpp"
#include "preferences.hpp"
#include "to_string.hpp"

// TODO adhere to prefs::saves::generate_defaults, prefs::saves::embed_tilesets

using namespace tactile::core;

namespace tactile::service {
namespace {

[[nodiscard]] auto save_tilesets(const tileset_manager& tilesets,
                                 const QString& destination,
                                 const export_options& options) -> QJsonArray
{
  QJsonArray array;

  for (const auto& [id, tileset] : tilesets) {
    QJsonObject object;

    object.insert(QStringLiteral(u"firstgid"), tileset.first_id().get());
    object.insert(QStringLiteral(u"name"), tileset.name());
    object.insert(QStringLiteral(u"tilewidth"), tileset.get_tile_width().get());
    object.insert(QStringLiteral(u"tileheight"),
                  tileset.get_tile_height().get());
    object.insert(QStringLiteral(u"tilecount"), tileset.tile_count());
    object.insert(QStringLiteral(u"columns"), tileset.cols().get());
    object.insert(QStringLiteral(u"imagewidth"), tileset.width());
    object.insert(QStringLiteral(u"imageheight"), tileset.height());
    object.insert(QStringLiteral(u"margin"), 0);
    object.insert(QStringLiteral(u"spacing"), 0);

    const QFileInfo info{destination};
    object.insert(QStringLiteral(u"image"),
                  info.dir().relativeFilePath(tileset.path()));

    if (options.generateDefaults) {
      object.insert(QStringLiteral(u"objectalignment"),
                    QStringLiteral(u"unspecified"));
    }

    array.append(object);
  }

  return array;
}

[[nodiscard]] auto save_layers(const map& map, const export_options& options)
    -> QJsonArray
{
  QJsonArray array;

  for (int id{1}; const auto& layer : map) {
    QJsonObject object;

    if (const auto str = to_string(id); str) {
      const auto name = "Layer " + *str;
      object.insert(QStringLiteral(u"name"), QString::fromStdString(name));
    }

    object.insert(QStringLiteral(u"width"), layer.cols().get());
    object.insert(QStringLiteral(u"height"), layer.rows().get());
    object.insert(QStringLiteral(u"id"), id);
    object.insert(QStringLiteral(u"opacity"), 1.0);
    //    object.insert(QStringLiteral(u"properties"), QJsonArray{});
    object.insert(QStringLiteral(u"type"), QStringLiteral(u"tilelayer"));
    object.insert(QStringLiteral(u"visible"), layer.visible());
    object.insert(QStringLiteral(u"x"), 0);
    object.insert(QStringLiteral(u"y"), 0);

    if (options.generateDefaults) {
      object.insert(QStringLiteral(u"encoding"), QStringLiteral(u"csv"));
      object.insert(QStringLiteral(u"compression"), QStringLiteral(u""));
      object.insert(QStringLiteral(u"draworder"), QStringLiteral(u"topdown"));
      object.insert(QStringLiteral(u"offsetx"), 0.0);
      object.insert(QStringLiteral(u"offsety"), 0.0);
    }

    QJsonArray data;
    layer.for_each([&](tile_id tile) { data.append(tile.get()); });
    object.insert(QStringLiteral(u"data"), data);

    array.append(object);
    ++id;
  }

  return array;
}

[[nodiscard]] auto create_root(const map& map,
                               const tileset_manager& tilesets,
                               const QString& path,
                               const export_options& options) -> QJsonObject
{
  QJsonObject root;

  // TODO better way to specify version
  root.insert(QStringLiteral(u"tiledversion"), QStringLiteral(u"1.4.2"));
  root.insert(QStringLiteral(u"orientation"), QStringLiteral(u"orthogonal"));
  root.insert(QStringLiteral(u"renderorder"), QStringLiteral(u"right-down"));
  root.insert(QStringLiteral(u"width"), map.cols().get());
  root.insert(QStringLiteral(u"height"), map.rows().get());
  root.insert(QStringLiteral(u"compressionlevel"), -1);
  root.insert(QStringLiteral(u"infinite"), false);
  root.insert(QStringLiteral(u"type"), QStringLiteral(u"map"));

  // TODO better way to specify version
  root.insert(QStringLiteral(u"version"), 1.4);
  root.insert(QStringLiteral(u"tilewidth"), prefs::saves::tile_width().value());
  root.insert(QStringLiteral(u"tileheight"),
              prefs::saves::tile_height().value());
  root.insert(QStringLiteral(u"nextobjectid"), 1);
  root.insert(QStringLiteral(u"nextlayerid"), map.num_layers() + 1);
  root.insert(QStringLiteral(u"tilesets"),
              save_tilesets(tilesets, path, options));
  root.insert(QStringLiteral(u"layers"), save_layers(map, options));

  return root;
}

}  // namespace

void save_json(const QString& path,
               const map& map,
               const tileset_manager& tilesets)
{
  export_options options;
  options.generateDefaults = prefs::saves::generate_defaults().value_or(false);
  options.embedTilesets = prefs::saves::embed_tilesets().value_or(true);

  QJsonDocument document{};
  document.setObject(create_root(map, tilesets, path, options));

  QFile file{path};
  file.open(QFile::WriteOnly);
  file.write(document.toJson());
}

}  // namespace tactile::service
