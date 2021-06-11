#include "save_map_document_as_json.hpp"

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

namespace tactile {
namespace {

[[nodiscard]] auto save_property(const QString& name,
                                 const core::Property& property,
                                 const QDir& targetDir) -> QJsonObject
{
  QJsonObject result;

  result.insert(u"name", name);
  result.insert(u"type", core::Stringify(property.Type().value()));

  QJsonValue value;
  switch (property.Type().value())
  {
    case core::PropertyType::string:
    {
      value = property.AsString();
      break;
    }
    case core::PropertyType::integer:
    {
      value = property.AsInteger();
      break;
    }
    case core::PropertyType::floating:
    {
      value = property.AsFloating();
      break;
    }
    case core::PropertyType::boolean:
    {
      value = property.AsBoolean();
      break;
    }
    case core::PropertyType::file:
    {
      value = targetDir.relativeFilePath(property.AsFile().filePath());
      break;
    }
    case core::PropertyType::color:
    {
      value = property.AsColor().name(QColor::HexArgb);
      break;
    }
    case core::PropertyType::object:
    {
      value = property.AsObject().get();
      break;
    }
  }

  result.insert(u"value", value);

  return result;
}

[[nodiscard]] auto save_properties(const core::IPropertyManager& manager,
                                   const QDir& targetDir) -> QJsonArray
{
  QJsonArray props;

  for (const auto& [name, property] : manager.GetProperties())
  {
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
                           const core::Tileset& tileset,
                           const QDir& targetDir,
                           const export_options& options)
{
  object.insert(u"columns", tileset.ColumnCount().get());

  object.insert(u"image", targetDir.relativeFilePath(tileset.AbsolutePath()));
  object.insert(u"imagewidth", tileset.Width());
  object.insert(u"imageheight", tileset.Height());
  object.insert(u"margin", 0);
  object.insert(u"name", tileset.Name());
  object.insert(u"spacing", 0);
  object.insert(u"tilecount", tileset.TileCount());
  object.insert(u"tilewidth", tileset.GetTileWidth().get());
  object.insert(u"tileheight", tileset.GetTileHeight().get());

  if (options.generateDefaults)
  {
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
[[nodiscard]] auto make_embedded_tileset_object(const core::Tileset& tileset,
                                                const QDir& targetDir,
                                                const export_options& options)
    -> QJsonObject
{
  QJsonObject object{};

  add_common_attributes(object, tileset, targetDir, options);
  object.insert(u"firstgid", tileset.FirstId().get());

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
[[nodiscard]] auto make_external_tileset_object(const core::Tileset& tileset)
    -> QJsonObject
{
  QJsonObject object{};

  object.insert(u"firstgid", tileset.FirstId().get());
  object.insert(u"source", tileset.Name() + TACTILE_QSTRING(u".json"));

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
void create_external_tileset_file(const core::Tileset& tileset,
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

  WriteJson(QFileInfo{targetDir.absoluteFilePath(
                      tileset.Name() + TACTILE_QSTRING(u".json"))},
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
[[nodiscard]] auto save_tilesets(const core::MapDocument& map,
                                 const QDir& targetDir,
                                 const export_options& options) -> QJsonArray
{
  QJsonArray array;

  map.EachTileset([&](tileset_id id, const core::Tileset& tileset) {
    if (options.embedTilesets)
    {
      array.append(make_embedded_tileset_object(tileset, targetDir, options));
    }
    else
    {
      array.append(make_external_tileset_object(tileset));
      create_external_tileset_file(tileset, targetDir, options);
    }
  });

  return array;
}

void save_tile_layer(QJsonObject& object,
                     const shared<core::ILayer>& layer,
                     const export_options& options)
{
  Q_ASSERT(layer->Type() == core::LayerType::tile_layer);

  const auto* tileLayer = core::AsTileLayer(layer);
  Q_ASSERT(tileLayer);

  object.insert(u"type", TACTILE_QSTRING(u"tilelayer"));

  if (options.generateDefaults)
  {
    object.insert(u"compression", QString{});
    object.insert(u"encoding", TACTILE_QSTRING(u"csv"));
  }

  QJsonArray data;
  tileLayer->Each([&](const tile_id tile) { data.append(tile.get()); });

  object.insert(u"data", data);
}

void save_object_layer(QJsonObject& element,
                       const shared<core::ILayer>& layer,
                       const QDir& targetDir,
                       const export_options& options)
{
  Q_ASSERT(layer->Type() == core::LayerType::object_layer);

  const auto* objectLayer = core::AsObjectLayer(layer);
  Q_ASSERT(objectLayer);

  element.insert(u"type", TACTILE_QSTRING(u"objectgroup"));

  if (options.generateDefaults)
  {
    element.insert(u"draworder", TACTILE_QSTRING(u"topdown"));
  }

  QJsonArray objects;
  objectLayer->Each([&](const object_id id, const core::Object& object) {
    QJsonObject jsonObject;

    jsonObject.insert(u"id", id.get());
    jsonObject.insert(u"name", object.Name());
    jsonObject.insert(u"x", object.X());
    jsonObject.insert(u"y", object.Y());
    jsonObject.insert(u"width", object.Width());
    jsonObject.insert(u"height", object.Height());
    jsonObject.insert(u"visible", object.IsVisible());
    jsonObject.insert(u"type", object.CustomType().value_or(""));
    jsonObject.insert(u"rotation", 0);

    if (object.IsPoint())
    {
      jsonObject.insert(u"point", true);
    }

    if (object.PropertyCount() > 0)
    {
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
[[nodiscard]] auto save_layers(const core::MapDocument& map,
                               const QDir& targetDir,
                               const export_options& options) -> QJsonArray
{
  QJsonArray array;

  map.EachLayer([&](const layer_id id, const shared<core::ILayer>& layer) {
    QJsonObject object;

    object.insert(u"id", id.get());
    object.insert(u"name", layer->Name());
    object.insert(u"opacity", layer->Opacity());
    object.insert(u"visible", layer->IsVisible());
    object.insert(u"x", 0);
    object.insert(u"y", 0);

    if (layer->Type() == core::LayerType::tile_layer)
    {
      object.insert(u"width", map.ColumnCount().get());
      object.insert(u"height", map.RowCount().get());
      save_tile_layer(object, layer, options);
    }
    else
    {
      save_object_layer(object, layer, targetDir, options);
    }

    if (options.generateDefaults)
    {
      object.insert(u"offsetx", 0.0);
      object.insert(u"offsety", 0.0);
    }

    if (layer->PropertyCount() > 0)
    {
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
[[nodiscard]] auto create_root(const core::MapDocument& map,
                               const QDir& targetDir,
                               const export_options& options) -> QJsonObject
{
  QJsonObject root{};

  root.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  root.insert(u"orientation", TACTILE_QSTRING(u"orthogonal"));
  root.insert(u"renderorder", TACTILE_QSTRING(u"right-down"));
  root.insert(u"width", map.ColumnCount().get());
  root.insert(u"height", map.RowCount().get());
  root.insert(u"compressionlevel", -1);
  root.insert(u"infinite", false);
  root.insert(u"type", TACTILE_QSTRING(u"map"));
  root.insert(u"version", TACTILE_TILED_JSON_VERSION_LITERAL);
  root.insert(u"tilewidth", prefs::TileWidth().value());
  root.insert(u"tileheight", prefs::TileHeight().value());
  root.insert(u"nextobjectid", 1);
  root.insert(u"nextlayerid", map.LayerCount() + 1);
  root.insert(u"tilesets", save_tilesets(map, targetDir, options));
  root.insert(u"layers", save_layers(map, targetDir, options));
  root.insert(u"properties", save_properties(map, targetDir));

  return root;
}

}  // namespace

void save_map_document_as_json(const QString& path,
                               const core::MapDocument& map)
{
  QJsonDocument document{};

  const QFileInfo info{path};
  const auto targetDir = info.dir();

  document.setObject(create_root(map, targetDir, make_export_options()));
  WriteJson(QFileInfo{path}, document);
}

}  // namespace tactile
