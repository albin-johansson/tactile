#include "save_json.hpp"

#include <QDir>           // QDir
#include <QFileInfo>      // QFileInfo
#include <QJsonArray>     // QJsonArray
#include <QJsonDocument>  // QJsonDocument
#include <QJsonObject>    // QJsonObject

#include "export_options.hpp"
#include "json_utils.hpp"
#include "layer_utils.hpp"
#include "object_layer.hpp"
#include "preferences.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "tiled_version.hpp"

using namespace tactile::core;

namespace tactile::service {
namespace {

[[nodiscard]] auto save_property(const QString& name,
                                 const core::property& property,
                                 const QDir& targetDir) -> QJsonObject
{
  QJsonObject result;

  result.insert(u"name", name);
  result.insert(u"type", core::stringify(property.get_type().value()));

  QJsonValue value;
  switch (property.get_type().value()) {
    case property::string: {
      value = property.as_string();
      break;
    }
    case property::integer: {
      value = property.as_integer();
      break;
    }
    case property::floating: {
      value = property.as_floating();
      break;
    }
    case property::boolean: {
      value = property.as_boolean();
      break;
    }
    case property::file: {
      value = targetDir.relativeFilePath(property.as_file().filePath());
      break;
    }
    case property::color: {
      value = property.as_color().name(QColor::HexArgb);
      break;
    }
    case property::object: {
      value = property.as_object().get();
      break;
    }
  }

  result.insert(u"value", value);

  return result;
}

[[nodiscard]] auto save_properties(const core::property_manager& manager,
                                   const QDir& targetDir) -> QJsonArray
{
  QJsonArray props;

  for (const auto& [name, property] : manager.properties()) {
    props.append(save_property(name, property, targetDir));
  }

  return props;
}

/**
 * \brief Adds common tileset attributes between embedded and external tilesets.
 *
 * \param object the JSON object to add the common attributes to.
 * \param tileset the tileset that will be saved.
 * \param targetDir the target directory of the map file.
 * \param options the export options that will be used.
 *
 * \since 0.1.0
 */
void add_common_attributes(QJsonObject& object,
                           const tileset& tileset,
                           const QDir& targetDir,
                           const export_options& options)
{
  object.insert(u"columns", tileset.col_count().get());

  object.insert(u"image", targetDir.relativeFilePath(tileset.absolute_path()));
  object.insert(u"imagewidth", tileset.width());
  object.insert(u"imageheight", tileset.height());
  object.insert(u"margin", 0);
  object.insert(u"name", tileset.name());
  object.insert(u"spacing", 0);
  object.insert(u"tilecount", tileset.tile_count());
  object.insert(u"tilewidth", tileset.get_tile_width().get());
  object.insert(u"tileheight", tileset.get_tile_height().get());

  if (options.generateDefaults) {
    object.insert(u"objectalignment", TACTILE_QSTRING(u"unspecified"));
  }
}

/**
 * \brief Creates a JSON object that represents an embedded tileset.
 *
 * \param tileset the tileset that will be saved.
 * \param targetDir the target directory of the map file.
 * \param options the export options that will be used.
 *
 * \return the created JSON object.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto make_embedded_tileset_object(const tileset& tileset,
                                                const QDir& targetDir,
                                                const export_options& options)
    -> QJsonObject
{
  QJsonObject object{};

  add_common_attributes(object, tileset, targetDir, options);
  object.insert(u"firstgid", tileset.first_id().get());

  return object;
}

/**
 * \brief Creates a JSON object that represents an external tileset.
 *
 * \note The returned JSON object should be present in the JSON *map* file, not
 * the actual external tileset file.
 *
 * \param tileset the tileset that will be saved.
 *
 * \return the created JSON object.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto make_external_tileset_object(const tileset& tileset)
    -> QJsonObject
{
  QJsonObject object{};

  object.insert(u"firstgid", tileset.first_id().get());
  object.insert(u"source", tileset.name() + TACTILE_QSTRING(u".json"));

  return object;
}

/**
 * \brief Creates an external JSON file that contains the data for a tileset.
 *
 * \details This function is used if the "Embed tilesets" option is disabled.
 * The tileset file will be stored next to the map file.
 *
 * \param tileset the tileset that will be saved.
 * \param targetDir the target directory of the map file.
 * \param options the export options that will be used.
 *
 * \since 0.1.0
 */
void create_external_tileset_file(const tileset& tileset,
                                  const QDir& targetDir,
                                  const export_options& options)
{
  QJsonDocument document{};
  QJsonObject object{};

  add_common_attributes(object, tileset, targetDir, options);
  object.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  object.insert(u"version", TACTILE_TILED_JSON_VERSION_LITERAL);
  object.insert(u"type", TACTILE_QSTRING(u"tileset"));

  document.setObject(object);

  json::write_file(
      targetDir.absoluteFilePath(tileset.name() + TACTILE_QSTRING(u".json")),
      document);
}

/**
 * \brief Creates a JSON array that contains the tilesets used by the map.
 *
 * \param map the map that will be saved.
 * \param targetDir the target directory of the map file.
 * \param options the export options that will be used.
 *
 * \return a JSON array of the tilesets.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto save_tilesets(const map_document& map,
                                 const QDir& targetDir,
                                 const export_options& options) -> QJsonArray
{
  QJsonArray array;

  map.each_tileset([&](tileset_id id, const tileset& tileset) {
    if (options.embedTilesets) {
      array.append(make_embedded_tileset_object(tileset, targetDir, options));
    } else {
      array.append(make_external_tileset_object(tileset));
      create_external_tileset_file(tileset, targetDir, options);
    }
  });

  return array;
}

void save_tile_layer(QJsonObject& object,
                     const shared<core::layer>& layer,
                     const export_options& options)
{
  Q_ASSERT(layer->type() == core::layer_type::tile_layer);

  const auto* tileLayer = core::as_tile_layer(layer);
  Q_ASSERT(tileLayer);

  object.insert(u"type", TACTILE_QSTRING(u"tilelayer"));

  if (options.generateDefaults) {
    object.insert(u"compression", TACTILE_QSTRING(u""));
    object.insert(u"encoding", TACTILE_QSTRING(u"csv"));
  }

  QJsonArray data;
  tileLayer->for_each([&](const tile_id tile) {
    data.append(tile.get());
  });

  object.insert(u"data", data);
}

void save_object_layer(QJsonObject& element,
                       const shared<core::layer>& layer,
                       const QDir& targetDir,
                       const export_options& options)
{
  Q_ASSERT(layer->type() == core::layer_type::object_layer);

  const auto* objectLayer = core::as_object_layer(layer);
  Q_ASSERT(objectLayer);

  element.insert(u"type", TACTILE_QSTRING(u"objectgroup"));

  if (options.generateDefaults) {
    element.insert(u"draworder", TACTILE_QSTRING(u"topdown"));
  }

  QJsonArray objects;
  objectLayer->each_object([&](const object_id id, const core::object& object) {
    QJsonObject jsonObject;

    jsonObject.insert(u"id", id.get());
    jsonObject.insert(u"name", object.name());
    jsonObject.insert(u"x", object.x());
    jsonObject.insert(u"y", object.y());
    jsonObject.insert(u"width", object.width());
    jsonObject.insert(u"height", object.height());
    jsonObject.insert(u"visible", object.visible());
    jsonObject.insert(u"type", object.custom_type().value_or(""));
    jsonObject.insert(u"rotation", 0);

    if (object.is_point()) {
      jsonObject.insert(u"point", true);
    }

    if (object.property_count() > 0) {
      jsonObject.insert(u"properties", save_properties(object, targetDir));
    }

    objects.append(jsonObject);
  });

  element.insert(u"objects", objects);
}

/**
 * \brief Creates a JSON array that contains the layers of a map.
 *
 * \param map the map that contains the layers that will be saved.
 * \param targetDir the target directory of the map file.
 * \param options the export options that will be used.
 *
 * \return a JSON array of the map layers.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto save_layers(const map_document& map,
                               const QDir& targetDir,
                               const export_options& options) -> QJsonArray
{
  QJsonArray array;

  map.each_layer([&](const layer_id id, const shared<core::layer>& layer) {
    QJsonObject object;

    object.insert(u"id", id.get());
    object.insert(u"name", layer->name());
    object.insert(u"opacity", layer->opacity());
    object.insert(u"visible", layer->visible());
    object.insert(u"x", 0);
    object.insert(u"y", 0);

    if (layer->type() == core::layer_type::tile_layer) {
      object.insert(u"width", map.col_count().get());
      object.insert(u"height", map.row_count().get());
      save_tile_layer(object, layer, options);
    } else {
      save_object_layer(object, layer, targetDir, options);
    }

    if (options.generateDefaults) {
      object.insert(u"offsetx", 0.0);
      object.insert(u"offsety", 0.0);
    }

    if (layer->property_count() > 0) {
      object.insert(u"properties", save_properties(*layer, targetDir));
    }

    array.append(object);
  });

  return array;
}

/**
 * \brief Creates the root JSON object for the map file.
 *
 * \param map the map that will be saved.
 * \param targetDir the target directory of the map file.
 * \param options the export options that will be used.
 *
 * \return the root JSON object.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto create_root(const map_document& map,
                               const QDir& targetDir,
                               const export_options& options) -> QJsonObject
{
  QJsonObject root{};

  root.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  root.insert(u"orientation", TACTILE_QSTRING(u"orthogonal"));
  root.insert(u"renderorder", TACTILE_QSTRING(u"right-down"));
  root.insert(u"width", map.col_count().get());
  root.insert(u"height", map.row_count().get());
  root.insert(u"compressionlevel", -1);
  root.insert(u"infinite", false);
  root.insert(u"type", TACTILE_QSTRING(u"map"));
  root.insert(u"version", TACTILE_TILED_JSON_VERSION_LITERAL);
  root.insert(u"tilewidth", prefs::saves::tile_width().value());
  root.insert(u"tileheight", prefs::saves::tile_height().value());
  root.insert(u"nextobjectid", 1);
  root.insert(u"nextlayerid", map.layer_count() + 1);
  root.insert(u"tilesets", save_tilesets(map, targetDir, options));
  root.insert(u"layers", save_layers(map, targetDir, options));
  root.insert(u"properties", save_properties(map, targetDir));

  return root;
}

}  // namespace

void save_json(const QString& path, const core::map_document& map)
{
  QJsonDocument document{};

  const QFileInfo info{path};
  const auto targetDir = info.dir();

  document.setObject(create_root(map, targetDir, make_export_options()));
  json::write_file(path, document);
}

}  // namespace tactile::service
