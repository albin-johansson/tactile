#include "save_tmx.hpp"

#include <QDir>
#include <QFileInfo>
#include <QtXml>

#include "export_options.hpp"
#include "preferences.hpp"
#include "tactile_qstring.hpp"
#include "tiled_version.hpp"
#include "xml_utils.hpp"

using namespace tactile::core;

namespace tactile::service {
namespace {

void add_image_node(QDomDocument& document,
                    QDomElement& parent,
                    const tileset& tileset,
                    const QFileInfo& mapInfo)
{
  auto image = document.createElement(TACTILE_QSTRING(u"image"));

  image.setAttribute(TACTILE_QSTRING(u"source"),
                     mapInfo.dir().relativeFilePath(tileset.file_path()));
  image.setAttribute(TACTILE_QSTRING(u"width"), tileset.width());
  image.setAttribute(TACTILE_QSTRING(u"height"), tileset.height());

  parent.appendChild(image);
}

void add_common_attributes(QDomDocument& document,
                           QDomElement& node,
                           const tileset& tileset,
                           const QFileInfo& mapInfo)
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

  add_image_node(document, node, tileset, mapInfo);
}

void create_external_tileset_file(const tileset& tileset,
                                  const QFileInfo& mapInfo,
                                  const export_options& options)
{
  QDomDocument document{};

  auto node = document.createElement(TACTILE_QSTRING(u"tileset"));
  add_common_attributes(document, node, tileset, mapInfo);

  document.appendChild(node);

  xml::write_file(mapInfo.absoluteDir().absoluteFilePath(
                      tileset.name() + TACTILE_QSTRING(u".tsx")),
                  document);
}

void save_tilesets(QDomDocument& document,
                   QDomElement& root,
                   const core::map_document& map,
                   const QFileInfo& mapInfo,
                   const export_options& options)
{
  map.each_tileset([&](tileset_id id, const tileset& tileset) {
    auto node = document.createElement(TACTILE_QSTRING(u"tileset"));

    node.setAttribute(TACTILE_QSTRING(u"firstgid"), tileset.first_id().get());

    if (options.embedTilesets) {
      add_common_attributes(document, node, tileset, mapInfo);
    } else {
      const auto source = mapInfo.dir().relativeFilePath(
          tileset.name() + TACTILE_QSTRING(u".tsx"));
      node.setAttribute(TACTILE_QSTRING(u"source"), source);
      create_external_tileset_file(tileset, mapInfo, options);
    }

    root.appendChild(node);
  });
}

void save_layers(QDomDocument& document,
                 QDomElement& root,
                 const map_document& map)
{
  map.each_layer([&](const layer_id id, const shared_layer& layer) {
    auto node = document.createElement(TACTILE_QSTRING(u"layer"));

    node.setAttribute(TACTILE_QSTRING(u"id"), id.get());
    node.setAttribute(TACTILE_QSTRING(u"name"), layer->name());

    if (const auto* tileLayer =
            dynamic_cast<const core::tile_layer*>(layer.get())) {
      node.setAttribute(TACTILE_QSTRING(u"width"),
                        tileLayer->col_count().get());
      node.setAttribute(TACTILE_QSTRING(u"height"),
                        tileLayer->row_count().get());
    }

    if (layer->opacity() != 1.0) {
      node.setAttribute(TACTILE_QSTRING(u"opacity"), layer->opacity());
    }

    if (!layer->visible()) {
      node.setAttribute(TACTILE_QSTRING(u"visible"), 0);
    }

    auto data = document.createElement(TACTILE_QSTRING(u"data"));
    data.setAttribute(TACTILE_QSTRING(u"encoding"), TACTILE_QSTRING(u"csv"));

    // FIXME
    if (const auto* tileLayer =
            dynamic_cast<const core::tile_layer*>(layer.get())) {
      QString buffer;

      // include the separating comma
      buffer.reserve(tileLayer->tile_count() * 2);

      bool first{true};
      tileLayer->for_each([&](const tile_id tile) {
        if (first) {
          first = false;
        } else {
          buffer += TACTILE_QSTRING(u",");
        }
        buffer += QString::number(tile.get());
      });
      data.appendChild(document.createTextNode(buffer));
    }

    node.appendChild(data);
    root.appendChild(node);
  });
}

void create_root(QDomDocument& document,
                 const core::map_document& map,
                 const QFileInfo& mapInfo,
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

  if (options.generateDefaults) {
    root.setAttribute(TACTILE_QSTRING(u"compressionlevel"), -1);
    root.setAttribute(TACTILE_QSTRING(u"backgroundcolor"),
                      TACTILE_QSTRING(u"#00000000"));
  }

  save_tilesets(document, root, map, mapInfo, options);
  save_layers(document, root, map);

  document.appendChild(root);
}

}  // namespace

void save_tmx(const QString& path, const core::map_document& map)
{
  const auto options = make_export_options();
  const QFileInfo info{path};

  QDomDocument document{};
  create_root(document, map, info, options);

  xml::write_file(path, document);
}

}  // namespace tactile::service
