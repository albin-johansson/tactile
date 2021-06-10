#include "save_map_document_as_xml.hpp"

#include <QDir>
#include <QFileInfo>
#include <QtXml>

#include "export_options.hpp"
#include "layer_utils.hpp"
#include "object_layer.hpp"
#include "preferences.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"
#include "tiled_version.hpp"
#include "xml_utils.hpp"

namespace tactile {
namespace {

void add_image_node(QDomDocument& document,
                    QDomElement& parent,
                    const core::tileset& tileset,
                    const QDir& targetDir)
{
  auto image = document.createElement(TACTILE_QSTRING(u"image"));

  image.setAttribute(TACTILE_QSTRING(u"source"),
                     targetDir.relativeFilePath(tileset.file_path()));
  image.setAttribute(TACTILE_QSTRING(u"width"), tileset.width());
  image.setAttribute(TACTILE_QSTRING(u"height"), tileset.height());

  parent.appendChild(image);
}

void add_common_attributes(QDomDocument& document,
                           QDomElement& node,
                           const core::tileset& tileset,
                           const QDir& targetDir)
{
  node.setAttribute(TACTILE_QSTRING(u"version"), g_tiledXmlVersion);
  node.setAttribute(TACTILE_QSTRING(u"tiledversion"), g_tiledVersion);
  node.setAttribute(TACTILE_QSTRING(u"name"), tileset.name());
  node.setAttribute(TACTILE_QSTRING(u"tilewidth"),
                    tileset.get_tile_width().get());
  node.setAttribute(TACTILE_QSTRING(u"tileheight"),
                    tileset.get_tile_height().get());
  node.setAttribute(TACTILE_QSTRING(u"tilecount"), tileset.tile_count());
  node.setAttribute(TACTILE_QSTRING(u"columns"), tileset.col_count().get());

  add_image_node(document, node, tileset, targetDir);
}

void create_external_tileset_file(const core::tileset& tileset,
                                  const QDir& targetDir,
                                  const export_options& options)
{
  QDomDocument document{};

  auto node = document.createElement(TACTILE_QSTRING(u"tileset"));
  add_common_attributes(document, node, tileset, targetDir);

  document.appendChild(node);

  xml::write_file(QFileInfo{targetDir.absoluteFilePath(
                      tileset.name() + TACTILE_QSTRING(u".tsx"))},
                  document);
}

void save_property(QDomDocument& document,
                   QDomElement& element,
                   const QString& name,
                   const core::property& property,
                   const QDir& targetDir)
{
  auto node = document.createElement(TACTILE_QSTRING(u"property"));

  node.setAttribute(TACTILE_QSTRING(u"name"), name);
  node.setAttribute(TACTILE_QSTRING(u"type"),
                    core::stringify(property.type().value()));

  switch (property.type().value())
  {
    case core::property_type::string:
    {
      node.setAttribute(TACTILE_QSTRING(u"value"), property.as_string());
      break;
    }
    case core::property_type::integer:
    {
      node.setAttribute(TACTILE_QSTRING(u"value"), property.as_integer());
      break;
    }
    case core::property_type::floating:
    {
      node.setAttribute(TACTILE_QSTRING(u"value"), property.as_floating());
      break;
    }
    case core::property_type::boolean:
    {
      node.setAttribute(TACTILE_QSTRING(u"value"), property.as_boolean());
      break;
    }
    case core::property_type::file:
    {
      node.setAttribute(
          TACTILE_QSTRING(u"value"),
          targetDir.relativeFilePath(property.as_file().filePath()));
      break;
    }
    case core::property_type::color:
    {
      node.setAttribute(TACTILE_QSTRING(u"value"),
                        property.as_color().name(QColor::HexArgb));
      break;
    }
    case core::property_type::object:
    {
      node.setAttribute(TACTILE_QSTRING(u"value"), property.as_object().get());
      break;
    }
  }

  element.appendChild(node);
}

void save_properties(QDomDocument& document,
                     QDomElement& element,
                     const core::property_manager& manager,
                     const QDir& targetDir)
{
  if (manager.property_count() > 0)
  {
    auto node = document.createElement(TACTILE_QSTRING(u"properties"));

    for (const auto& [name, property] : manager.properties())
    {
      save_property(document, node, name, property, targetDir);
    }

    element.appendChild(node);
  }
}

void save_tilesets(QDomDocument& document,
                   QDomElement& root,
                   const core::map_document& map,
                   const QDir& targetDir,
                   const export_options& options)
{
  map.each_tileset([&](tileset_id id, const core::tileset& tileset) {
    auto node = document.createElement(TACTILE_QSTRING(u"tileset"));

    node.setAttribute(TACTILE_QSTRING(u"firstgid"), tileset.first_id().get());

    if (options.embedTilesets)
    {
      add_common_attributes(document, node, tileset, targetDir);
    }
    else
    {
      const auto source =
          targetDir.relativeFilePath(tileset.name() + TACTILE_QSTRING(u".tsx"));
      node.setAttribute(TACTILE_QSTRING(u"source"), source);
      create_external_tileset_file(tileset, targetDir, options);
    }

    root.appendChild(node);
  });
}

void save_tile_layer(QDomDocument& document,
                     QDomElement& element,
                     const core::TileLayer& layer)
{
  element.setAttribute(TACTILE_QSTRING(u"width"), layer.ColumnCount().get());
  element.setAttribute(TACTILE_QSTRING(u"height"), layer.RowCount().get());

  auto data = document.createElement(TACTILE_QSTRING(u"data"));
  data.setAttribute(TACTILE_QSTRING(u"encoding"), TACTILE_QSTRING(u"csv"));

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
      buffer += TACTILE_QSTRING(u",");
    }
    buffer += QString::number(tile.get());
  });

  data.appendChild(document.createTextNode(buffer));
  element.appendChild(data);
}

void save_object_layer(QDomDocument& document,
                       QDomElement& element,
                       const core::object_layer& layer,
                       const QDir& targetDir)
{
  layer.each_object([&](const object_id id, const core::object& object) {
    auto node = document.createElement(TACTILE_QSTRING(u"object"));

    node.setAttribute(TACTILE_QSTRING(u"id"), id.get());
    node.setAttribute(TACTILE_QSTRING(u"name"), object.name());
    node.setAttribute(TACTILE_QSTRING(u"x"), object.x());
    node.setAttribute(TACTILE_QSTRING(u"y"), object.y());
    node.setAttribute(TACTILE_QSTRING(u"width"), object.width());
    node.setAttribute(TACTILE_QSTRING(u"height"), object.height());
    node.setAttribute(TACTILE_QSTRING(u"visible"), object.visible());
    node.setAttribute(TACTILE_QSTRING(u"type"),
                      object.custom_type().value_or(""));

    if (object.is_point())
    {
      auto point = document.createElement(TACTILE_QSTRING(u"point"));
      node.appendChild(point);
    }

    save_properties(document, node, object, targetDir);

    element.appendChild(node);
  });
}

void save_layers(QDomDocument& document,
                 QDomElement& root,
                 const core::map_document& map,
                 const QDir& targetDir)
{
  map.each_layer([&](const layer_id id, const shared<core::ILayer>& layer) {
    QDomElement node;
    if (layer->Type() == core::LayerType::tile_layer)
    {
      node = document.createElement(TACTILE_QSTRING(u"layer"));
    }
    else
    {
      node = document.createElement(TACTILE_QSTRING(u"objectgroup"));
    }

    node.setAttribute(TACTILE_QSTRING(u"id"), id.get());
    node.setAttribute(TACTILE_QSTRING(u"name"), layer->Name());

    if (layer->Opacity() != 1.0)
    {
      node.setAttribute(TACTILE_QSTRING(u"opacity"), layer->Opacity());
    }

    if (!layer->IsVisible())
    {
      node.setAttribute(TACTILE_QSTRING(u"visible"), 0);
    }

    if (const auto* tileLayer = core::as_tile_layer(layer))
    {
      save_tile_layer(document, node, *tileLayer);
    }

    if (const auto* objectLayer = core::as_object_layer(layer))
    {
      save_object_layer(document, node, *objectLayer, targetDir);
    }

    save_properties(document, node, *layer, targetDir);

    root.appendChild(node);
  });
}

void create_root(QDomDocument& document,
                 const core::map_document& map,
                 const QDir& targetDir,
                 const export_options& options)
{
  auto root = document.createElement(TACTILE_QSTRING(u"map"));

  root.setAttribute(TACTILE_QSTRING(u"version"), g_tiledXmlVersion);
  root.setAttribute(TACTILE_QSTRING(u"tiledversion"), g_tiledVersion);
  root.setAttribute(TACTILE_QSTRING(u"orientation"),
                    TACTILE_QSTRING(u"orthogonal"));
  root.setAttribute(TACTILE_QSTRING(u"renderorder"),
                    TACTILE_QSTRING(u"right-down"));
  root.setAttribute(TACTILE_QSTRING(u"width"), map.col_count().get());
  root.setAttribute(TACTILE_QSTRING(u"height"), map.row_count().get());
  root.setAttribute(TACTILE_QSTRING(u"tilewidth"),
                    prefs::saves::tile_width().value());
  root.setAttribute(TACTILE_QSTRING(u"tileheight"),
                    prefs::saves::tile_height().value());
  root.setAttribute(TACTILE_QSTRING(u"infinite"), 0);
  root.setAttribute(TACTILE_QSTRING(u"nextlayerid"), map.layer_count() + 1);
  root.setAttribute(TACTILE_QSTRING(u"nextobjectid"), 1);

  if (options.generateDefaults)
  {
    root.setAttribute(TACTILE_QSTRING(u"compressionlevel"), -1);
    root.setAttribute(TACTILE_QSTRING(u"backgroundcolor"),
                      TACTILE_QSTRING(u"#00000000"));
  }

  save_tilesets(document, root, map, targetDir, options);
  save_layers(document, root, map, targetDir);
  save_properties(document, root, map, targetDir);

  document.appendChild(root);
}

}  // namespace

void save_map_document_as_xml(const QString& path,
                              const core::map_document& map)
{
  const auto options = make_export_options();

  const QFileInfo info{path};
  const auto targetDir = info.dir();

  QDomDocument document{};
  create_root(document, map, targetDir, options);

  xml::write_file(info, document);
}

}  // namespace tactile
