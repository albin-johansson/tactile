#include "json_element.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::tmx {

json_element::json_element(QJsonObject object) : m_object{std::move(object)}
{}

auto json_element::contains(const QString& str) const -> bool
{
  return m_object.contains(str);
}

auto json_element::integer(const QString& str) const -> maybe<int>
{
  if (const auto result = m_object.value(str).toInt(-1); result != -1) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto json_element::integer(const element_type id) const -> maybe<int>
{
  if (const auto result = m_object.value(stringify_element_id(id)).toInt(-1);
      result != -1) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto json_element::integer(const QString& str, const int def) const -> int
{
  return integer(str).value_or(def);
}

auto json_element::floating(const QString& str, const double def) const
    -> double
{
  if (const auto result = m_object.value(str).toDouble(-1); result != -1) {
    return result;
  } else {
    return def;
  }
}

auto json_element::string(const QString& str) const -> maybe<QString>
{
  const auto result = m_object.value(str).toString();
  if (!result.isNull()) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto json_element::string(const QString& str, const QString& def) const
    -> QString
{
  return string(str).value_or(def);
}

auto json_element::stringify_element_id(const element_type type) -> QStringView
{
  switch (type) {
    case element_type::tileset:
      return u"tileset";

    case element_type::tilesets:
      throw tactile_error{"JSON format does not feature \"tilesets\"!"};

    case element_type::next_layer_id:
      return u"nextlayerid";

    case element_type::source:
      return u"source";

    case element_type::first_gid:
      return u"firstgid";

    case element_type::tile_width:
      return u"tilewidth";

    case element_type::tile_height:
      return u"tileheight";

    case element_type::image:
      return u"image";

    case element_type::name:
      return u"name";

    case element_type::id:
      return u"id";

    case element_type::width:
      return u"width";

    case element_type::height:
      return u"height";

    case element_type::visible:
      return u"visible";

    case element_type::opacity:
      return u"opacity";

    default:
      throw tactile_error{"Reached end of switch statement!"};
  }
}

}  // namespace tactile::tmx
