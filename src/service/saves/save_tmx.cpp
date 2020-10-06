#include "save_tmx.hpp"

#include <qdir.h>
#include <qfileinfo.h>

#include <QtXml>

#include "export_options.hpp"
#include "preferences.hpp"
#include "tiled_version.hpp"

using namespace tactile::core;

namespace tactile::service {
namespace {

void add_image_node(QDomDocument& document,
                    QDomElement& parent,
                    const tileset& tileset,
                    const QFileInfo& mapInfo)
{
  auto image = document.createElement(QStringLiteral(u"image"));

  image.setAttribute(QStringLiteral(u"source"),
                     mapInfo.dir().relativeFilePath(tileset.path()));
  image.setAttribute(QStringLiteral(u"width"), tileset.width());
  image.setAttribute(QStringLiteral(u"height"), tileset.height());

  parent.appendChild(image);
}

void add_common_attributes(QDomDocument& document,
                           QDomElement& node,
                           const tileset& tileset,
                           const QFileInfo& mapInfo)
{
  node.setAttribute(QStringLiteral(u"version"), g_tiledXmlVersion);
  node.setAttribute(QStringLiteral(u"tiledversion"), g_tiledVersion);
  node.setAttribute(QStringLiteral(u"name"), tileset.name());
  node.setAttribute(QStringLiteral(u"tilewidth"),
                    tileset.get_tile_width().get());
  node.setAttribute(QStringLiteral(u"tileheight"),
                    tileset.get_tile_height().get());
  node.setAttribute(QStringLiteral(u"tilecount"), tileset.tile_count());
  node.setAttribute(QStringLiteral(u"columns"), tileset.cols().get());

  add_image_node(document, node, tileset, mapInfo);
}

void create_external_tileset_file(const tileset& tileset,
                                  const QFileInfo& mapInfo,
                                  const export_options& options)
{
  QDomDocument document{};

  auto node = document.createElement(QStringLiteral(u"tileset"));
  add_common_attributes(document, node, tileset, mapInfo);

  document.appendChild(node);

  QFile file{mapInfo.absoluteDir().absoluteFilePath(tileset.name() +
                                                    QStringLiteral(u".tsx"))};
  file.open(QIODevice::WriteOnly);

  QTextStream stream{&file};
  stream << document;
  file.close();
}

void save_tilesets(QDomDocument& document,
                   QDomElement& root,
                   const tileset_manager& tilesets,
                   const QFileInfo& mapInfo,
                   const export_options& options)
{
  for (const auto& [id, tileset] : tilesets) {
    auto node = document.createElement(QStringLiteral(u"tileset"));

    node.setAttribute(QStringLiteral(u"firstgid"), tileset.first_id().get());

    if (options.embedTilesets) {
      add_common_attributes(document, node, tileset, mapInfo);
    } else {
      const auto source = mapInfo.dir().relativeFilePath(
          tileset.name() + QStringLiteral(u".tsx"));
      node.setAttribute(QStringLiteral(u"source"), source);
      create_external_tileset_file(tileset, mapInfo, options);
    }

    root.appendChild(node);
  }
}

void save_layers(QDomDocument& document, QDomElement& root, const map& map)
{
  int id{1};
  for (const auto& layer : map) {
    auto node = document.createElement(QStringLiteral(u"layer"));

    node.setAttribute(QStringLiteral(u"id"), id);
    node.setAttribute(QStringLiteral(u"name"),
                      QStringLiteral(u"Layer ") + QString::number(id));
    node.setAttribute(QStringLiteral(u"width"), layer.cols().get());
    node.setAttribute(QStringLiteral(u"height"), layer.rows().get());

    if (!layer.visible()) {
      node.setAttribute(QStringLiteral(u"visible"), 0);
    }

    auto data = document.createElement(QStringLiteral(u"data"));
    data.setAttribute(QStringLiteral(u"encoding"), QStringLiteral(u"csv"));

    QString buffer;
    const auto count = map.rows().get() * map.cols().get();
    buffer.reserve(count);

    bool first{true};
    layer.for_each([&](tile_id tile) {
      if (first) {
        first = false;
      } else {
        buffer += QStringLiteral(u",");
      }
      buffer += QString::number(tile.get());
    });

    data.appendChild(document.createTextNode(buffer));
    node.appendChild(data);
    root.appendChild(node);

    ++id;
  }
}

void create_root(QDomDocument& document,
                 const map& map,
                 const tileset_manager& tilesets,
                 const QFileInfo& mapInfo,
                 const export_options& options)
{
  auto root = document.createElement(QStringLiteral(u"map"));

  root.setAttribute(QStringLiteral(u"version"), g_tiledXmlVersion);
  root.setAttribute(QStringLiteral(u"tiledversion"), g_tiledVersion);
  root.setAttribute(QStringLiteral(u"orientation"),
                    QStringLiteral(u"orthogonal"));
  root.setAttribute(QStringLiteral(u"renderorder"),
                    QStringLiteral(u"right-down"));
  root.setAttribute(QStringLiteral(u"width"), map.cols().get());
  root.setAttribute(QStringLiteral(u"height"), map.rows().get());
  root.setAttribute(QStringLiteral(u"tilewidth"),
                    prefs::saves::tile_width().value());
  root.setAttribute(QStringLiteral(u"tileheight"),
                    prefs::saves::tile_height().value());
  root.setAttribute(QStringLiteral(u"infinite"), 0);
  root.setAttribute(QStringLiteral(u"nextlayerid"), map.num_layers() + 1);
  root.setAttribute(QStringLiteral(u"nextobjectid"), 1);

  if (options.generateDefaults) {
    root.setAttribute(QStringLiteral(u"compressionlevel"), -1);
    root.setAttribute(QStringLiteral(u"backgroundcolor"),
                      QStringLiteral(u"#00000000"));
  }

  save_tilesets(document, root, tilesets, mapInfo, options);
  save_layers(document, root, map);

  document.appendChild(root);
}

}  // namespace

void save_tmx(const QString& path,
              const map& map,
              const tileset_manager& tilesets)
{
  const auto options = make_export_options();
  const QFileInfo info{path};

  QDomDocument document{};
  create_root(document, map, tilesets, info, options);

  QFile file{path};
  file.open(QIODevice::WriteOnly);

  QTextStream stream{&file};
  stream << document;
  file.close();
}

}  // namespace tactile::service
