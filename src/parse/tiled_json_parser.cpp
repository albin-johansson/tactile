#include "tiled_json_parser.hpp"

#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <ranges>   // all_of
#include <utility>  // exchange

#include "json_utils.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto get_layer_visibility(const QJsonObject& object) -> bool
{
  if (!object.contains(u"visible")) {
    qWarning("Layer did not feature \"visible\" property!");
  }
  return object.value(u"visible").toBool(true);
}

[[nodiscard]] auto get_layer_opacity(const QJsonObject& object) -> double
{
  if (!object.contains(u"opacity")) {
    qWarning("Layer did not feature \"opacity\" property!");
  }
  return object.value(u"opacity").toDouble(1.0);
}

[[nodiscard]] auto get_layer_name(const QJsonObject& object) -> QString
{
  if (!object.contains(u"name")) {
    qWarning("Layer did not feature \"name\" property!");
  }
  return object.value(u"name").toString(QStringLiteral(u"N/A"));
}

[[nodiscard]] auto index_to_position(const int index, const int nCols)
    -> core::position
{
  return core::position{row_t{index / nCols}, col_t{index % nCols}};
}

}  // namespace

tiled_json_parser::tiled_json_parser(const QFileInfo& path)
{
  if (const auto json = open_file(path); json) {
    m_document = new core::map_document{};

    const auto root = json->object();
    if (!parse_next_layer_id(root)) {
      return;
    }

    if (!parse_tilesets(root, path)) {
      return;
    }

    if (!parse_layers(root)) {
      return;
    }
  }
}

tiled_json_parser::~tiled_json_parser() noexcept
{
  delete m_document;
}

auto tiled_json_parser::take_document() -> core::map_document*
{
  return std::exchange(m_document, nullptr);
}

auto tiled_json_parser::error_code() const -> parse_error
{
  return m_error;
}

tiled_json_parser::operator bool() const noexcept
{
  return m_error == parse_error::none;
}

auto tiled_json_parser::open_file(const QFileInfo& path)
    -> std::optional<QJsonDocument>
{
  if (!path.exists()) {
    m_error = parse_error::map_file_not_found;
    return std::nullopt;
  }

  auto json = json::from_file(path);

  if (!json) {
    m_error = parse_error::could_not_parse_json;
  }

  return json;
}

auto tiled_json_parser::parse_next_layer_id(const QJsonObject& root) -> bool
{
  if (const auto result = root.value(u"nextlayerid").toInt(-1); result != -1) {
    m_document->set_next_layer_id(layer_id{result});
    return true;
  } else {
    return set_error(parse_error::map_missing_next_layer_id);
  }
}

auto tiled_json_parser::parse_tilesets(const QJsonObject& root,
                                       const QFileInfo& path) -> bool
{
  if (!root.contains(u"tilesets")) {
    return set_error(parse_error::map_missing_tilesets);
  }

  const auto array = root.value(u"tilesets").toArray();

  return std::ranges::all_of(array, [&, this](const QJsonValue& elem) {
    const auto object = elem.toObject();

    const auto firstGid = parse_tileset_first_gid(object);
    if (!firstGid) {
      return false;
    }

    if (object.contains(u"source")) {
      return parse_external_tileset(object, path, *firstGid);
    } else {
      return parse_tileset_common(object, path, *firstGid);
    }
  });
}

auto tiled_json_parser::parse_tileset_first_gid(const QJsonObject& object)
    -> std::optional<tile_id>
{
  const auto firstGid = object.value(u"firstgid").toInt(-1);
  if (firstGid != -1) {
    return tile_id{firstGid};
  } else {
    m_error = parse_error::tileset_missing_first_gid;
    return std::nullopt;
  }
}

auto tiled_json_parser::parse_external_tileset(const QJsonObject& object,
                                               const QFileInfo& path,
                                               const tile_id firstGid) -> bool
{
  const auto source = object.value(u"source").toString();

  const auto external = json::from_file(path.dir().absoluteFilePath(source));
  if (external) {
    return parse_tileset_common(external->object(), path, firstGid);
  } else {
    return set_error(parse_error::could_not_read_external_tileset);
  }
}

auto tiled_json_parser::parse_tileset_common(const QJsonObject& object,
                                             const QFileInfo& path,
                                             const tile_id firstGid) -> bool
{
  const auto tw = object.value(u"tilewidth").toInt(-1);
  if (tw == -1) {
    return set_error(parse_error::tileset_missing_tile_width);
  }

  const auto th = object.value(u"tileheight").toInt(-1);
  if (th == -1) {
    return set_error(parse_error::tileset_missing_tile_height);
  }

  const auto relativePath = object.value(u"image").toString();
  if (relativePath.isNull()) {
    return set_error(parse_error::tileset_missing_image_path);
  }

  const auto absolutePath = path.dir().absoluteFilePath(relativePath);
  if (!QFileInfo{absolutePath}.exists()) {
    return set_error(parse_error::external_tileset_does_not_exist);
  }

  try {
    auto tileset = std::make_shared<core::tileset>(firstGid,
                                                   absolutePath,
                                                   tile_width{tw},
                                                   tile_height{th});

    const auto name = object.value(u"name").toString();
    if (name.isNull()) {
      return set_error(parse_error::tileset_missing_name);
    }

    tileset->set_name(name);
    tileset->set_path(absolutePath);

    m_document->add_tileset(std::move(tileset));
  } catch (...) {
    return set_error(parse_error::could_not_create_tileset);
  }

  return true;
}

auto tiled_json_parser::parse_layers(const QJsonObject& root) -> bool
{
  if (!root.contains(u"layers")) {
    return set_error(parse_error::map_missing_layers);
  }

  bool first{true};
  const auto array = root.value(u"layers").toArray();
  for (const auto& elem : array) {
    const auto object = elem.toObject();

    if (!object.contains(u"type")) {
      return set_error(parse_error::layer_missing_type);
    }

    if (object.value(u"type").toString() != QStringView{u"tilelayer"}) {
      qWarning("Skipping layer in file because it was not a tile layer!");
      continue;
    }

    const auto id = object.value(u"id").toInt(-1);
    if (id == -1) {
      return set_error(parse_error::layer_missing_id);
    }

    const auto height = object.value(u"height").toInt(-1);
    if (height == -1) {
      return set_error(parse_error::layer_missing_height);
    }

    const auto width = object.value(u"width").toInt(-1);
    if (width == -1) {
      return set_error(parse_error::layer_missing_width);
    }

    auto layer = std::make_shared<core::layer>(row_t{height}, col_t{width});
    layer->set_visible(get_layer_visibility(object));
    layer->set_opacity(get_layer_opacity(object));
    layer->set_name(get_layer_name(object));

    const auto nCols = layer->col_count().get();
    const auto data = object.value(u"data").toArray();
    for (int index{0}; const auto value : data) {
      const auto tileId = value.toInt(-1);
      if (tileId == -1) {
        return set_error(parse_error::layer_could_not_parse_tile);
      }

      layer->set_tile(index_to_position(index, nCols), tile_id{tileId});
      ++index;
    }

    m_document->add_layer(layer_id{id}, layer);

    if (first) {
      first = false;
      m_document->select_layer(layer_id{id});
    }
  }

  return true;
}

}  // namespace tactile
