#include "save_map_document_as_xml.hpp"

#include <QDir>
#include <QFileInfo>
#include <QtXml>

#include "export_options.hpp"
#include "layer_utils.hpp"
#include "object_layer.hpp"
#include "preferences.hpp"
#include "tile_layer.hpp"
#include "tiled_version.hpp"
#include "xml_utils.hpp"

namespace tactile {
namespace {

void SaveProperty(QDomDocument& document,
                  QDomElement& element,
                  const QString& name,
                  const core::Property& property,
                  const QDir& targetDir)
{
  auto node = document.createElement(QStringLiteral(u"property"));

  node.setAttribute(QStringLiteral(u"name"), name);
  node.setAttribute(QStringLiteral(u"type"),
                    core::Stringify(property.Type().value()));

  switch (property.Type().value())
  {
    case core::PropertyType::String:
    {
      node.setAttribute(QStringLiteral(u"value"), property.AsString());
      break;
    }
    case core::PropertyType::Integer:
    {
      node.setAttribute(QStringLiteral(u"value"), property.AsInteger());
      break;
    }
    case core::PropertyType::Floating:
    {
      node.setAttribute(QStringLiteral(u"value"), property.AsFloating());
      break;
    }
    case core::PropertyType::Boolean:
    {
      node.setAttribute(QStringLiteral(u"value"), property.AsBoolean());
      break;
    }
    case core::PropertyType::File:
    {
      node.setAttribute(
          QStringLiteral(u"value"),
          targetDir.relativeFilePath(property.AsFile().filePath()));
      break;
    }
    case core::PropertyType::Color:
    {
      node.setAttribute(QStringLiteral(u"value"),
                        property.AsColor().name(QColor::HexArgb));
      break;
    }
    case core::PropertyType::Object:
    {
      node.setAttribute(QStringLiteral(u"value"), property.AsObject().get());
      break;
    }
  }

  element.appendChild(node);
}

void SaveProperties(QDomDocument& document,
                    QDomElement& element,
                    const core::IPropertyManager& manager,
                    const QDir& targetDir)
{
  if (manager.PropertyCount() > 0)
  {
    auto node = document.createElement(QStringLiteral(u"properties"));

    for (const auto& [name, property] : manager.GetProperties())
    {
      SaveProperty(document, node, name, property, targetDir);
    }

    element.appendChild(node);
  }
}

void AddImageNode(QDomDocument& document,
                  QDomElement& parent,
                  const core::Tileset& tileset,
                  const QDir& targetDir)
{
  auto image = document.createElement(QStringLiteral(u"image"));

  image.setAttribute(QStringLiteral(u"source"),
                     targetDir.relativeFilePath(tileset.FilePath()));
  image.setAttribute(QStringLiteral(u"width"), tileset.Width());
  image.setAttribute(QStringLiteral(u"height"), tileset.Height());

  parent.appendChild(image);
}

void AddCommonAttributes(QDomDocument& document,
                         QDomElement& node,
                         const core::Tileset& tileset,
                         const QDir& targetDir)
{
  node.setAttribute(QStringLiteral(u"version"), tiled_xml_version);
  node.setAttribute(QStringLiteral(u"tiledversion"), tiled_version);
  node.setAttribute(QStringLiteral(u"name"), tileset.Name());
  node.setAttribute(QStringLiteral(u"tilewidth"), tileset.GetTileWidth().get());
  node.setAttribute(QStringLiteral(u"tileheight"),
                    tileset.GetTileHeight().get());
  node.setAttribute(QStringLiteral(u"tilecount"), tileset.TileCount());
  node.setAttribute(QStringLiteral(u"columns"), tileset.ColumnCount().get());

  AddImageNode(document, node, tileset, targetDir);
  SaveProperties(document, node, tileset, targetDir);
}

void CreateExternalTilesetFile(const core::Tileset& tileset,
                               const QDir& targetDir,
                               const ExportOptions& options)
{
  QDomDocument document{};

  auto node = document.createElement(QStringLiteral(u"tileset"));
  AddCommonAttributes(document, node, tileset, targetDir);

  document.appendChild(node);

  WriteXml(QFileInfo{targetDir.absoluteFilePath(tileset.Name() +
                                                QStringLiteral(u".tsx"))},
           document);
}

void SaveTilesets(QDomDocument& document,
                  QDomElement& root,
                  const core::MapDocument& map,
                  const QDir& targetDir,
                  const ExportOptions& options)
{
  map.EachTileset([&](tileset_id id, const core::Tileset& tileset) {
    auto node = document.createElement(QStringLiteral(u"tileset"));

    node.setAttribute(QStringLiteral(u"firstgid"), tileset.FirstId().get());

    if (options.embedTilesets)
    {
      AddCommonAttributes(document, node, tileset, targetDir);
    }
    else
    {
      const auto source =
          targetDir.relativeFilePath(tileset.Name() + QStringLiteral(u".tsx"));
      node.setAttribute(QStringLiteral(u"source"), source);
      CreateExternalTilesetFile(tileset, targetDir, options);
    }

    root.appendChild(node);
  });
}

void SaveTileLayer(QDomDocument& document,
                   QDomElement& element,
                   const core::TileLayer& layer)
{
  element.setAttribute(QStringLiteral(u"width"), layer.ColumnCount().get());
  element.setAttribute(QStringLiteral(u"height"), layer.RowCount().get());

  auto data = document.createElement(QStringLiteral(u"data"));
  data.setAttribute(QStringLiteral(u"encoding"), QStringLiteral(u"csv"));

  QString buffer;

  // include the separating comma
  buffer.reserve(layer.TileCount() * 2);

  bool first{true};
  layer.Each([&](const tile_id tile) {
    if (first)
    {
      first = false;
    }
    else
    {
      buffer += QStringLiteral(u",");
    }
    buffer += QString::number(tile.get());
  });

  data.appendChild(document.createTextNode(buffer));
  element.appendChild(data);
}

void SaveObjectLayer(QDomDocument& document,
                     QDomElement& element,
                     const core::ObjectLayer& layer,
                     const QDir& targetDir)
{
  layer.Each([&](const object_id id, const core::Object& object) {
    auto node = document.createElement(QStringLiteral(u"object"));

    node.setAttribute(QStringLiteral(u"id"), id.get());
    node.setAttribute(QStringLiteral(u"name"), object.Name());
    node.setAttribute(QStringLiteral(u"x"), object.X());
    node.setAttribute(QStringLiteral(u"y"), object.Y());
    node.setAttribute(QStringLiteral(u"width"), object.Width());
    node.setAttribute(QStringLiteral(u"height"), object.Height());
    node.setAttribute(QStringLiteral(u"visible"), object.IsVisible());
    node.setAttribute(QStringLiteral(u"type"),
                      object.CustomType().value_or(""));

    if (object.IsPoint())
    {
      auto point = document.createElement(QStringLiteral(u"point"));
      node.appendChild(point);
    }

    SaveProperties(document, node, object, targetDir);

    element.appendChild(node);
  });
}

void SaveLayers(QDomDocument& document,
                QDomElement& root,
                const core::MapDocument& map,
                const QDir& targetDir)
{
  map.EachLayer([&](const layer_id id, const Shared<core::ILayer>& layer) {
    QDomElement node;
    if (layer->Type() == core::LayerType::TileLayer)
    {
      node = document.createElement(QStringLiteral(u"layer"));
    }
    else
    {
      node = document.createElement(QStringLiteral(u"objectgroup"));
    }

    node.setAttribute(QStringLiteral(u"id"), id.get());
    node.setAttribute(QStringLiteral(u"name"), layer->GetName());

    if (layer->Opacity() != 1.0)
    {
      node.setAttribute(QStringLiteral(u"opacity"), layer->Opacity());
    }

    if (!layer->IsVisible())
    {
      node.setAttribute(QStringLiteral(u"visible"), 0);
    }

    if (const auto* tileLayer = core::AsTileLayer(layer))
    {
      SaveTileLayer(document, node, *tileLayer);
    }

    if (const auto* objectLayer = core::AsObjectLayer(layer))
    {
      SaveObjectLayer(document, node, *objectLayer, targetDir);
    }

    SaveProperties(document, node, *layer, targetDir);

    root.appendChild(node);
  });
}

void CreateRoot(QDomDocument& document,
                const core::MapDocument& map,
                const QDir& targetDir,
                const ExportOptions& options)
{
  auto root = document.createElement(QStringLiteral(u"map"));

  root.setAttribute(QStringLiteral(u"version"), tiled_xml_version);
  root.setAttribute(QStringLiteral(u"tiledversion"), tiled_version);
  root.setAttribute(QStringLiteral(u"orientation"),
                    QStringLiteral(u"orthogonal"));
  root.setAttribute(QStringLiteral(u"renderorder"),
                    QStringLiteral(u"right-down"));
  root.setAttribute(QStringLiteral(u"width"), map.ColumnCount().get());
  root.setAttribute(QStringLiteral(u"height"), map.RowCount().get());
  root.setAttribute(QStringLiteral(u"tilewidth"), prefs::TileWidth().Value());
  root.setAttribute(QStringLiteral(u"tileheight"), prefs::TileHeight().Value());
  root.setAttribute(QStringLiteral(u"infinite"), 0);
  root.setAttribute(QStringLiteral(u"nextlayerid"), map.LayerCount() + 1);
  root.setAttribute(QStringLiteral(u"nextobjectid"), 1);

  if (options.generateDefaults)
  {
    root.setAttribute(QStringLiteral(u"compressionlevel"), -1);
    root.setAttribute(QStringLiteral(u"backgroundcolor"),
                      QStringLiteral(u"#00000000"));
  }

  SaveTilesets(document, root, map, targetDir, options);
  SaveLayers(document, root, map, targetDir);
  SaveProperties(document, root, map, targetDir);

  document.appendChild(root);
}

}  // namespace

void SaveMapDocumentAsXml(const QString& path, const core::MapDocument& map)
{
  const auto options = MakeExportOptions();

  const QFileInfo info{path};
  const auto targetDir = info.dir();

  QDomDocument document;
  CreateRoot(document, map, targetDir, options);

  WriteXml(info, document);
}

}  // namespace tactile
