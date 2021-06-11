#include "json_element.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile::parse {

json_element::json_element(QJsonObject object) : m_object{std::move(object)}
{}

auto json_element::contains(const ElementId id) const -> bool
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

auto json_element::integer(const ElementId id) const -> maybe<int>
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

auto json_element::integer(const ElementId id, const int def) const
    -> maybe<int>
{
  return integer(id).value_or(def);
}

auto json_element::floating(const ElementId id) const -> maybe<double>
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

auto json_element::floating(const ElementId id, const double def) const
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

auto json_element::string(const ElementId id) const -> maybe<QString>
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

auto json_element::string(const ElementId id, const QString& def) const
    -> QString
{
  return string(id).value_or(def);
}

auto json_element::boolean(const ElementId id) const -> maybe<bool>
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

auto json_element::stringify_element_id(const ElementId type) -> QStringView
{
  switch (type)
  {
    case ElementId::Tileset:
      return u"tileset";

    case ElementId::Tilesets:
      return u"tilesets";

    case ElementId::NextLayerId:
      return u"nextlayerid";

    case ElementId::Source:
      return u"source";

    case ElementId::FirstGid:
      return u"firstgid";

    case ElementId::TileWidth:
      return u"tilewidth";

    case ElementId::TileHeight:
      return u"tileheight";

    case ElementId::Image:
      return u"image";

    case ElementId::Name:
      return u"name";

    case ElementId::Id:
      return u"id";

    case ElementId::Width:
      return u"width";

    case ElementId::Height:
      return u"height";

    case ElementId::Visible:
      return u"visible";

    case ElementId::Opacity:
      return u"opacity";

    case ElementId::Type:
      return u"type";

    case ElementId::Value:
      return u"value";

    case ElementId::X:
      return u"x";

    case ElementId::Y:
      return u"y";

    case ElementId::Point:
      return u"point";

    default:
      throw TactileError{"Reached end of switch statement!"};
  }
}

}  // namespace tactile::parse
