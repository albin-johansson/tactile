#include "save_json.hpp"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include "export_options.hpp"
#include "preferences.hpp"
#include "tiled_version.hpp"
#include "to_string.hpp"

using namespace tactile::core;

namespace tactile::service {
namespace {

/**
 * @brief Adds common tileset attributes between embedded and external tilesets.
 *
 * @param object the JSON object to add the common attributes to.
 * @param tileset the tileset that will be saved.
 * @param mapDestination the file path of the map file.
 * @param options the export options that will be used.
 *
 * @since 0.1.0
 */
void add_common_attributes(QJsonObject& object,
                           const tileset& tileset,
                           const QString& mapDestination,
                           const export_options& options)
{
  object.insert(u"columns", tileset.cols().get());

  const QFileInfo info{mapDestination};
  object.insert(u"image", info.dir().relativeFilePath(tileset.path()));
  object.insert(u"imagewidth", tileset.width());
  object.insert(u"imageheight", tileset.height());
  object.insert(u"margin", 0);
  object.insert(u"name", tileset.name());
  object.insert(u"spacing", 0);
  object.insert(u"tilecount", tileset.tile_count());
  object.insert(u"tilewidth", tileset.get_tile_width().get());
  object.insert(u"tileheight", tileset.get_tile_height().get());

  if (options.generateDefaults) {
    object.insert(u"objectalignment", QStringLiteral(u"unspecified"));
  }
}

/**
 * @brief Creates a JSON object that represents an embedded tileset.
 *
 * @param tileset the tileset that will be saved.
 * @param mapDestination the file path of the map file.
 * @param options the export options that will be used.
 *
 * @return the created JSON object.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto make_embedded_tileset_object(const tileset& tileset,
                                                const QString& mapDestination,
                                                const export_options& options)
    -> QJsonObject
{
  QJsonObject object;

  add_common_attributes(object, tileset, mapDestination, options);
  object.insert(u"firstgid", tileset.first_id().get());

  return object;
}

/**
 * @brief Creates a JSON object that represents an external tileset.
 *
 * @note The returned JSON object should be present in the JSON *map* file, not
 * the actual external tileset file.
 *
 * @param tileset the tileset that will be saved.
 * @param mapDestination the file path of the map file.
 * @param options the export options that will be used.
 *
 * @return the created JSON object.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto make_external_tileset_object(const tileset& tileset,
                                                const QString& mapDestination,
                                                const export_options& options)
    -> QJsonObject
{
  QJsonObject object;

  object.insert(u"firstgid", tileset.first_id().get());
  object.insert(u"source", tileset.name() + QStringLiteral(u".json"));

  return object;
}

/**
 * @brief Creates an external JSON file that contains the data for a tileset.
 *
 * @details This function is used if the "Embed tilesets" option is disabled.
 * The tileset file will be stored next to the map file.
 *
 * @param tileset the tileset that will be saved.
 * @param mapDestination the file path of the map file, *not* the tileset file.
 * @param options the export options that will be used.
 *
 * @since 0.1.0
 */
void create_external_tileset_file(const tileset& tileset,
                                  const QString& mapDestination,
                                  const export_options& options)
{
  QJsonDocument document{};
  QJsonObject object;

  add_common_attributes(object, tileset, mapDestination, options);
  object.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  object.insert(u"version", g_tiledJsonVersion);
  object.insert(u"type", QStringLiteral(u"tileset"));

  document.setObject(object);

  const QFileInfo info{mapDestination};
  QFile file{info.absoluteDir().absoluteFilePath(tileset.name() +
                                                 QStringLiteral(u".json"))};
  file.open(QFile::WriteOnly);
  file.write(document.toJson());
}

/**
 * @brief Creates a JSON array that contains the tilesets used by the map.
 *
 * @param tilesets the tilesets used by the map.
 * @param mapDestination the file path of the map file.
 * @param options the export options that will be used.
 *
 * @return a JSON array of the tilesets.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto save_tilesets(const tileset_manager& tilesets,
                                 const QString& mapDestination,
                                 const export_options& options) -> QJsonArray
{
  QJsonArray array;

  for (const auto& [id, tileset] : tilesets) {
    if (options.embedTilesets) {
      array.append(
          make_embedded_tileset_object(tileset, mapDestination, options));
    } else {
      array.append(
          make_external_tileset_object(tileset, mapDestination, options));
      create_external_tileset_file(tileset, mapDestination, options);
    }
  }

  return array;
}

/**
 * @brief Creates a JSON array that contains the layers of a map.
 *
 * @param map the map that contains the layers that will be saved.
 * @param options the export options that will be used.
 *
 * @return a JSON array of the map layers.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto save_layers(const map& map, const export_options& options)
    -> QJsonArray
{
  QJsonArray array;

  int id{1};
  for (const auto& layer : map) {
    QJsonObject object;

    if (const auto str = to_string(id); str) {
      const auto name = "Layer " + *str;
      object.insert(u"name", QString::fromStdString(name));
    }

    object.insert(u"width", layer.cols().get());
    object.insert(u"height", layer.rows().get());
    object.insert(u"id", id);
    object.insert(u"opacity", 1.0);
    object.insert(u"type", QStringLiteral(u"tilelayer"));
    object.insert(u"visible", layer.visible());
    object.insert(u"x", 0);
    object.insert(u"y", 0);

    if (options.generateDefaults) {
      object.insert(u"encoding", QStringLiteral(u"csv"));
      object.insert(u"compression", QStringLiteral(u""));
      object.insert(u"draworder", QStringLiteral(u"topdown"));
      object.insert(u"offsetx", 0.0);
      object.insert(u"offsety", 0.0);
    }

    QJsonArray data;
    layer.for_each([&](tile_id tile) { data.append(tile.get()); });
    object.insert(u"data", data);

    array.append(object);
    ++id;
  }

  return array;
}

/**
 * @brief Creates the root JSON object for the map file.
 *
 * @param map the map that will be saved.
 * @param tilesets the tilesets that are used.
 * @param mapDestination the destination path of the map file.
 * @param options the export options that will be used.
 *
 * @return the root JSON object.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto create_root(const map& map,
                               const tileset_manager& tilesets,
                               const QString& mapDestination,
                               const export_options& options) -> QJsonObject
{
  QJsonObject root;

  root.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  root.insert(u"orientation", QStringLiteral(u"orthogonal"));
  root.insert(u"renderorder", QStringLiteral(u"right-down"));
  root.insert(u"width", map.cols().get());
  root.insert(u"height", map.rows().get());
  root.insert(u"compressionlevel", -1);
  root.insert(u"infinite", false);
  root.insert(u"type", QStringLiteral(u"map"));
  root.insert(u"version", g_tiledJsonVersion);
  root.insert(u"tilewidth", prefs::saves::tile_width().value());
  root.insert(u"tileheight", prefs::saves::tile_height().value());
  root.insert(u"nextobjectid", 1);
  root.insert(u"nextlayerid", map.num_layers() + 1);
  root.insert(u"tilesets", save_tilesets(tilesets, mapDestination, options));
  root.insert(u"layers", save_layers(map, options));

  return root;
}

}  // namespace

void save_json(const QString& path,
               const map& map,
               const tileset_manager& tilesets)
{
  const auto options = make_export_options();

  QJsonDocument document{};
  document.setObject(create_root(map, tilesets, path, options));

  QFile file{path};
  file.open(QFile::WriteOnly);
  file.write(document.toJson());
}

}  // namespace tactile::service
