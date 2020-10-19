#include "save_json.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "export_options.hpp"
#include "json_utils.hpp"
#include "preferences.hpp"
#include "tiled_version.hpp"

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
  object.insert(u"columns", tileset.col_count().get());

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
  QJsonObject object{};

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
  QJsonObject object{};

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
  QJsonObject object{};

  add_common_attributes(object, tileset, mapDestination, options);
  object.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  object.insert(u"version", TACTILE_TILED_JSON_VERSION_LITERAL);
  object.insert(u"type", QStringLiteral(u"tileset"));

  document.setObject(object);

  const QFileInfo info{mapDestination};
  json::write_file(info.absoluteDir().absoluteFilePath(
                       tileset.name() + QStringLiteral(u".json")),
                   document);
}

/**
 * @brief Creates a JSON array that contains the tilesets used by the map.
 *
 * @param map the map that will be saved.
 * @param mapDestination the file path of the map file.
 * @param options the export options that will be used.
 *
 * @return a JSON array of the tilesets.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto save_tilesets(const map_document& map,
                                 const QString& mapDestination,
                                 const export_options& options) -> QJsonArray
{
  QJsonArray array;

  map.each_tileset([&](tileset_id id, const tileset& tileset) {
    if (options.embedTilesets) {
      array.append(
          make_embedded_tileset_object(tileset, mapDestination, options));
    } else {
      array.append(
          make_external_tileset_object(tileset, mapDestination, options));
      create_external_tileset_file(tileset, mapDestination, options);
    }
  });

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
[[nodiscard]] auto save_layers(const map_document& map,
                               const export_options& options) -> QJsonArray
{
  QJsonArray array;

  map.each_layer([&](layer_id id, const layer& layer) {
    QJsonObject object;

    object.insert(u"name",
                  QStringLiteral(u"Layer ") + QString::number(id.get()));
    object.insert(u"width", layer.col_count().get());
    object.insert(u"height", layer.row_count().get());
    object.insert(u"id", id.get());
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
    layer.for_each([&](tile_id tile) {
      data.append(tile.get());
    });
    object.insert(u"data", data);

    array.append(object);
  });

  return array;
}

/**
 * @brief Creates the root JSON object for the map file.
 *
 * @param map the map that will be saved.
 * @param mapDestination the destination path of the map file.
 * @param options the export options that will be used.
 *
 * @return the root JSON object.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto create_root(const map_document& map,
                               const QString& mapDestination,
                               const export_options& options) -> QJsonObject
{
  QJsonObject root{};

  root.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  root.insert(u"orientation", QStringLiteral(u"orthogonal"));
  root.insert(u"renderorder", QStringLiteral(u"right-down"));
  root.insert(u"width", map.col_count().get());
  root.insert(u"height", map.row_count().get());
  root.insert(u"compressionlevel", -1);
  root.insert(u"infinite", false);
  root.insert(u"type", QStringLiteral(u"map"));
  root.insert(u"version", TACTILE_TILED_JSON_VERSION_LITERAL);
  root.insert(u"tilewidth", prefs::saves::tile_width().value());
  root.insert(u"tileheight", prefs::saves::tile_height().value());
  root.insert(u"nextobjectid", 1);
  root.insert(u"nextlayerid", map.layer_count() + 1);
  root.insert(u"tilesets", save_tilesets(map, mapDestination, options));
  root.insert(u"layers", save_layers(map, options));

  return root;
}

}  // namespace

void save_json(const QString& path, const core::map_document& map)
{
  QJsonDocument document{};
  document.setObject(create_root(map, path, make_export_options()));
  json::write_file(path, document);
}

}  // namespace tactile::service
