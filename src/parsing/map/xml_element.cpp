#include "xml_element.hpp"

#include <cassert>  // assert

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

XmlElement::XmlElement(const QDomElement& element) : mElement{element}
{}

auto XmlElement::Contains(const ElementId id) const -> bool
{
  return mElement.hasAttribute(StringifyElementId(id));
}

auto XmlElement::Integer(const ElementId id) const -> std::optional<int>
{
  bool ok;
  const auto result = mElement.attribute(StringifyElementId(id)).toInt(&ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto XmlElement::Integer(const ElementId id, const int def) const
    -> std::optional<int>
{
  return Integer(id).value_or(def);
}

auto XmlElement::Floating(const ElementId id) const -> std::optional<double>
{
  bool ok;
  const auto result = mElement.attribute(StringifyElementId(id)).toDouble(&ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto XmlElement::Floating(const ElementId id, const double def) const -> double
{
  return Floating(id).value_or(def);
}

auto XmlElement::String(const ElementId id) const -> std::optional<QString>
{
  const auto result = mElement.attribute(StringifyElementId(id));
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto XmlElement::String(const ElementId id, const QString& def) const -> QString
{
  return String(id).value_or(def);
}

auto XmlElement::Boolean(const ElementId id) const -> std::optional<bool>
{
  if (const auto value = String(id))
  {
    return value == TACTILE_QSTRING(u"true");
  }
  else
  {
    return std::nullopt;
  }
}

auto XmlElement::StringifyElementId(const ElementId type) -> QString
{
  switch (type)
  {
    case ElementId::Tileset:
      return TACTILE_QSTRING(u"tileset");

    case ElementId::Tilesets:
      throw TactileError{"TMX format does not feature \"tilesets\"!"};

    case ElementId::NextLayerId:
      return TACTILE_QSTRING(u"nextlayerid");

    case ElementId::Source:
      return TACTILE_QSTRING(u"source");

    case ElementId::FirstGid:
      return TACTILE_QSTRING(u"firstgid");

    case ElementId::TileWidth:
      return TACTILE_QSTRING(u"tilewidth");

    case ElementId::TileHeight:
      return TACTILE_QSTRING(u"tileheight");

    case ElementId::Image:
      return TACTILE_QSTRING(u"image");

    case ElementId::Name:
      return TACTILE_QSTRING(u"name");

    case ElementId::Id:
      return TACTILE_QSTRING(u"id");

    case ElementId::Width:
      return TACTILE_QSTRING(u"width");

    case ElementId::Height:
      return TACTILE_QSTRING(u"height");

    case ElementId::Visible:
      return TACTILE_QSTRING(u"visible");

    case ElementId::Opacity:
      return TACTILE_QSTRING(u"opacity");

    case ElementId::Type:
      return TACTILE_QSTRING(u"type");

    case ElementId::Value:
      return TACTILE_QSTRING(u"value");

    case ElementId::X:
      return TACTILE_QSTRING(u"x");

    case ElementId::Y:
      return TACTILE_QSTRING(u"y");

    case ElementId::Point:
      return TACTILE_QSTRING(u"point");

    default:
      throw TactileError{"Reached end of switch statement!"};
  }
}

}  // namespace tactile::parse
