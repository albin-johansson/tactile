#include "json_element.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile_error.hpp"

namespace tactile {

JsonElement::JsonElement(QJsonObject object) : mObject{std::move(object)}
{}

auto JsonElement::Contains(const ElementId id) const -> bool
{
  return mObject.contains(StringifyElementId(id));
}

auto JsonElement::Integer(const QString& str) const -> std::optional<int>
{
  if (const auto result = mObject.value(str).toInt(-1); result != -1)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto JsonElement::Integer(const QString& str, const int def) const -> int
{
  return Integer(str).value_or(def);
}

auto JsonElement::Integer(const ElementId id) const -> std::optional<int>
{
  if (const auto result = mObject.value(StringifyElementId(id)).toInt(-1);
      result != -1)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto JsonElement::Integer(const ElementId id, const int def) const
    -> std::optional<int>
{
  return Integer(id).value_or(def);
}

auto JsonElement::Floating(const ElementId id) const -> std::optional<double>
{
  if (const auto result = mObject.value(StringifyElementId(id)).toDouble(-1);
      result != -1)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto JsonElement::Floating(const QString& str, const double def) const -> double
{
  if (const auto result = mObject.value(str).toDouble(-1); result != -1)
  {
    return result;
  }
  else
  {
    return def;
  }
}

auto JsonElement::Floating(const ElementId id, const double def) const -> double
{
  return Floating(id).value_or(def);
}

auto JsonElement::String(const QString& str) const -> std::optional<QString>
{
  const auto result = mObject.value(str).toString();
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto JsonElement::String(const QString& str, const QString& def) const
    -> QString
{
  return String(str).value_or(def);
}

auto JsonElement::String(const ElementId id) const -> std::optional<QString>
{
  const auto result = mObject.value(StringifyElementId(id)).toString();
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto JsonElement::String(const ElementId id, const QString& def) const
    -> QString
{
  return String(id).value_or(def);
}

auto JsonElement::Boolean(const ElementId id) const -> std::optional<bool>
{
  const auto value = mObject.value(StringifyElementId(id));
  if (value.isBool())
  {
    return value.toBool();
  }
  else
  {
    return std::nullopt;
  }
}

auto JsonElement::StringifyElementId(ElementId type) -> QStringView
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

}  // namespace tactile
