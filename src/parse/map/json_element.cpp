#include "json_element.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::parse {

json_element::json_element(QJsonObject object) : m_object{std::move(object)}
{}

auto json_element::contains(const element_id id) const -> bool
{
  return m_object.contains(stringify_element_id(id));
}

auto json_element::integer(const QString& str) const -> maybe<int>
{
  if (const auto result = m_object.value(str).toInt(-1); result != -1)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto json_element::integer(const QString& str, const int def) const -> int
{
  return integer(str).value_or(def);
}

auto json_element::integer(const element_id id) const -> maybe<int>
{
  if (const auto result = m_object.value(stringify_element_id(id)).toInt(-1);
      result != -1)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto json_element::integer(const element_id id, const int def) const
    -> maybe<int>
{
  return integer(id).value_or(def);
}

auto json_element::floating(const element_id id) const -> maybe<double>
{
  if (const auto result = m_object.value(stringify_element_id(id)).toDouble(-1);
      result != -1)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto json_element::floating(const QString& str, const double def) const
    -> double
{
  if (const auto result = m_object.value(str).toDouble(-1); result != -1)
  {
    return result;
  }
  else
  {
    return def;
  }
}

auto json_element::floating(const element_id id, const double def) const
    -> double
{
  return floating(id).value_or(def);
}

auto json_element::string(const QString& str) const -> maybe<QString>
{
  const auto result = m_object.value(str).toString();
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto json_element::string(const QString& str, const QString& def) const
    -> QString
{
  return string(str).value_or(def);
}

auto json_element::string(const element_id id) const -> maybe<QString>
{
  const auto result = m_object.value(stringify_element_id(id)).toString();
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto json_element::string(const element_id id, const QString& def) const
    -> QString
{
  return string(id).value_or(def);
}

auto json_element::boolean(const element_id id) const -> maybe<bool>
{
  const auto value = m_object.value(stringify_element_id(id));
  if (value.isBool())
  {
    return value.toBool();
  }
  else
  {
    return std::nullopt;
  }
}

auto json_element::stringify_element_id(const element_id type) -> QStringView
{
  switch (type)
  {
    case element_id::tileset:
      return u"tileset";

    case element_id::tilesets:
      return u"tilesets";

    case element_id::next_layer_id:
      return u"nextlayerid";

    case element_id::source:
      return u"source";

    case element_id::first_gid:
      return u"firstgid";

    case element_id::tile_width:
      return u"tilewidth";

    case element_id::tile_height:
      return u"tileheight";

    case element_id::image:
      return u"image";

    case element_id::name:
      return u"name";

    case element_id::id:
      return u"id";

    case element_id::width:
      return u"width";

    case element_id::height:
      return u"height";

    case element_id::visible:
      return u"visible";

    case element_id::opacity:
      return u"opacity";

    case element_id::type:
      return u"type";

    case element_id::value:
      return u"value";

    case element_id::x:
      return u"x";

    case element_id::y:
      return u"y";

    case element_id::point:
      return u"point";

    default:
      throw TactileError{"Reached end of switch statement!"};
  }
}

}  // namespace tactile::parse
