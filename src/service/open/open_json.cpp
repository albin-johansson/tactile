#include "open_json.hpp"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>   // make_shared
#include <utility>  // move

#include "json_utils.hpp"
#include "layer.hpp"
#include "tactile_error.hpp"
#include "tileset.hpp"

namespace tactile::service {
namespace {

void add_tileset_common(core::map_document* document,
                        const QJsonObject& object,
                        const QFileInfo& path,
                        tile_id gid)
{
  const tile_width tileWidth{object.value(u"tilewidth").toInt()};
  const tile_height tileHeight{object.value(u"tileheight").toInt()};

  const auto relativePath = object.value(u"image").toString();
  const auto absolutePath = path.dir().absoluteFilePath(relativePath);

  auto tileset =
      std::make_shared<core::tileset>(gid, absolutePath, tileWidth, tileHeight);

  tileset->set_name(object.value(u"name").toString());
  tileset->set_path(absolutePath);

  document->add_tileset(std::move(tileset));
}

void add_tilesets(core::map_document* document,
                  const QFileInfo& path,
                  const QJsonArray& tilesets)
{
  for (const auto& elem : tilesets) {
    const auto object = elem.toObject();

    const tile_id gid{object.value(u"firstgid").toInt()};

    if (object.contains(u"source")) {
      const auto source = object.value(u"source").toString();
      const auto external =
          json::from_file(path.dir().absoluteFilePath(source));
      add_tileset_common(document, external.object(), path, gid);
    } else {
      add_tileset_common(document, object, path, gid);
    }
  }
}

void add_layers(core::map_document* document, const QJsonArray& layers)
{
  bool first{true};
  for (const auto& elem : layers) {
    const auto object = elem.toObject();
    if (object.value(u"type").toString() != QStringView{u"tilelayer"}) {
      continue;
    }

    const layer_id id{object.value(u"id").toInt(-1)};
    const row_t rows{object.value(u"height").toInt(-1)};
    const col_t cols{object.value(u"width").toInt(-1)};

    auto layer = std::make_shared<core::layer>(rows, cols);
    layer->set_visible(object.value(u"visible").toBool(true));
    layer->set_opacity(object.value(u"opacity").toDouble(1.0));
    layer->set_name(object.value(u"name").toString());

    const auto data = object.value(u"data").toArray();
    for (int index{0}; const auto value : data) {
      const core::position pos{row_t{index / cols.get()},
                               col_t{index % cols.get()}};
      const tile_id tile{value.toInt(empty.get())};

      layer->set_tile(pos, tile);
      ++index;
    }

    document->add_layer(id, layer);

    if (first) {
      first = false;
      document->select_layer(id);
    }
  }
}

}  // namespace

auto open_json_map(const QFileInfo& path) -> core::map_document*
{
  Q_ASSERT(path.exists());

  const auto json = json::from_file(path.absoluteFilePath());
  const auto root = json.object();

  auto* document = new core::map_document{};
  document->set_next_layer_id(layer_id{root.value(u"nextlayerid").toInt(1)});

  add_tilesets(document, path, root.value(u"tilesets").toArray());
  add_layers(document, root.value(u"layers").toArray());

  return document;
}

}  // namespace tactile::service
