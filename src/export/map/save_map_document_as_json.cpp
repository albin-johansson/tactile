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

[[nodiscard]] auto SaveProperty(const QString& name,
                                const core::Property& property,
                                const QDir& targetDir) -> QJsonObject
{
  QJsonObject result;

  result.insert(u"name", name);
  result.insert(u"type", core::Stringify(property.Type().value()));

  QJsonValue value;
  switch (property.Type().value())
  {
    case core::PropertyType::String:
    {
      value = property.AsString();
      break;
    }
    case core::PropertyType::Integer:
    {
      value = property.AsInteger();
      break;
    }
    case core::PropertyType::Floating:
    {
      value = property.AsFloating();
      break;
    }
    case core::PropertyType::Boolean:
    {
      value = property.AsBoolean();
      break;
    }
    case core::PropertyType::File:
    {
      value = targetDir.relativeFilePath(property.AsFile().filePath());
      break;
    }
    case core::PropertyType::Color:
    {
      value = property.AsColor().name(QColor::HexArgb);
      break;
    }
    case core::PropertyType::Object:
    {
      value = property.AsObject().get();
      break;
    }
  }

  result.insert(u"value", value);

  return result;
}

[[nodiscard]] auto SaveProperties(const core::IPropertyManager& manager,
                                  const QDir& targetDir) -> QJsonArray
{
  QJsonArray props;

  for (const auto& [name, property] : manager.GetProperties())
  {
    props.append(SaveProperty(name, property, targetDir));
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
void AddCommonAttributes(QJsonObject& object,
                         const core::Tileset& tileset,
                         const QDir& targetDir,
                         const ExportOptions& options)
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
[[nodiscard]] auto MakeEmbeddedTilesetObject(const core::Tileset& tileset,
                                             const QDir& targetDir,
                                             const ExportOptions& options)
    -> QJsonObject
{
  QJsonObject object;

  AddCommonAttributes(object, tileset, targetDir, options);
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
[[nodiscard]] auto MakeExternalTilesetObject(const core::Tileset& tileset)
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
void CreateExternalTilesetFile(const core::Tileset& tileset,
                               const QDir& targetDir,
                               const ExportOptions& options)
{
  QJsonDocument document{};
  QJsonObject object{};

  AddCommonAttributes(object, tileset, targetDir, options);
  object.insert(u"tiledversion", TACTILE_TILED_VERSION_LITERAL);
  object.insert(u"version", TACTILE_TILED_JSON_VERSION_LITERAL);
  object.insert(u"type", TACTILE_QSTRING(u"tileset"));

  document.setObject(object);

  WriteJson(QFileInfo{targetDir.absoluteFilePath(tileset.Name() +
                                                 TACTILE_QSTRING(u".json"))},
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
[[nodiscard]] auto SaveTilesets(const core::MapDocument& map,
                                const QDir& targetDir,
                                const ExportOptions& options) -> QJsonArray
{
  QJsonArray array;

  map.EachTileset([&](tileset_id id, const core::Tileset& tileset) {
    if (options.embedTilesets)
    {
      array.append(MakeEmbeddedTilesetObject(tileset, targetDir, options));
    }
    else
    {
      array.append(MakeExternalTilesetObject(tileset));
      CreateExternalTilesetFile(tileset, targetDir, options);
    }
  });

  return array;
}

void SaveTileLayer(QJsonObject& object,
                   const Shared<core::ILayer>& layer,
                   const ExportOptions& options)
{
  Q_ASSERT(layer->Type() == core::LayerType::TileLayer);

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

void SaveObjectLayer(QJsonObject& element,
                     const Shared<core::ILayer>& layer,
                     const QDir& targetDir,
                     const ExportOptions& options)
{
  Q_ASSERT(layer->Type() == core::LayerType::ObjectLayer);

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
      jsonObject.insert(u"properties", SaveProperties(object, targetDir));
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
[[nodiscard]] auto SaveLayers(const core::MapDocument& map,
                              const QDir& targetDir,
                              const ExportOptions& options) -> QJsonArray
{
  QJsonArray array;

  map.EachLayer([&](const layer_id id, const Shared<core::ILayer>& layer) {
    QJsonObject object;

    object.insert(u"id", id.get());
    object.insert(u"name", layer->Name());
    object.insert(u"opacity", layer->Opacity());
    object.insert(u"visible", layer->IsVisible());
    object.insert(u"x", 0);
    object.insert(u"y", 0);

    if (layer->Type() == core::LayerType::TileLayer)
    {
      object.insert(u"width", map.ColumnCount().get());
      object.insert(u"height", map.RowCount().get());
      SaveTileLayer(object, layer, options);
    }
    else
    {
      SaveObjectLayer(object, layer, targetDir, options);
    }

    if (options.generateDefaults)
    {
      object.insert(u"offsetx", 0.0);
      object.insert(u"offsety", 0.0);
    }

    if (layer->PropertyCount() > 0)
    {
      object.insert(u"properties", SaveProperties(*layer, targetDir));
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
[[nodiscard]] auto CreateRoot(const core::MapDocument& map,
                              const QDir& targetDir,
                              const ExportOptions& options) -> QJsonObject
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
  root.insert(u"tilewidth", prefs::TileWidth().Value());
  root.insert(u"tileheight", prefs::TileHeight().Value());
  root.insert(u"nextobjectid", 1);
  root.insert(u"nextlayerid", map.LayerCount() + 1);
  root.insert(u"tilesets", SaveTilesets(map, targetDir, options));
  root.insert(u"layers", SaveLayers(map, targetDir, options));
  root.insert(u"properties", SaveProperties(map, targetDir));

  return root;
}

}  // namespace

void SaveMapDocumentAsJson(const QString& path, const core::MapDocument& map)
{
  QJsonDocument document{};

  const QFileInfo info{path};
  const auto targetDir = info.dir();

  document.setObject(CreateRoot(map, targetDir, MakeExportOptions()));
  WriteJson(QFileInfo{path}, document);
}

}  // namespace tactile
