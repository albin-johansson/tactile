#include "xml_element.hpp"

#include <cassert>  // assert

#include "tactile_error.hpp"

namespace tactile {

XmlElement::XmlElement(const QDomElement& element) : mElement{element}
{}

auto XmlElement::Contains(const ElementId id) const -> bool
{
  return mElement.hasAttribute(StringifyElementId(id));
}

auto XmlElement::Integer(const ElementId id) const -> Maybe<int>
{
  bool ok;
  const auto result = mElement.attribute(StringifyElementId(id)).toInt(&ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return nothing;
  }
}

auto XmlElement::Integer(const ElementId id, const int def) const -> Maybe<int>
{
  return Integer(id).value_or(def);
}

auto XmlElement::Floating(const ElementId id) const -> Maybe<double>
{
  bool ok;
  const auto result = mElement.attribute(StringifyElementId(id)).toDouble(&ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return nothing;
  }
}

auto XmlElement::Floating(const ElementId id, const double def) const -> double
{
  return Floating(id).value_or(def);
}

auto XmlElement::String(const ElementId id) const -> Maybe<QString>
{
  const auto result = mElement.attribute(StringifyElementId(id));
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return nothing;
  }
}

auto XmlElement::String(const ElementId id, const QString& def) const -> QString
{
  return String(id).value_or(def);
}

auto XmlElement::Boolean(const ElementId id) const -> Maybe<bool>
{
  if (const auto value = String(id))
  {
    return value == QStringLiteral(u"true");
  }
  else
  {
    return nothing;
  }
}

auto XmlElement::StringifyElementId(const ElementId type) -> QString
{
  switch (type)
  {
    case ElementId::Tileset:
      return QStringLiteral(u"tileset");

    case ElementId::Tilesets:
      throw TactileError{"TMX format does not feature \"tilesets\"!"};

    case ElementId::NextLayerId:
      return QStringLiteral(u"nextlayerid");

    case ElementId::Source:
      return QStringLiteral(u"source");

    case ElementId::FirstGid:
      return QStringLiteral(u"firstgid");

    case ElementId::TileWidth:
      return QStringLiteral(u"tilewidth");

    case ElementId::TileHeight:
      return QStringLiteral(u"tileheight");

    case ElementId::Image:
      return QStringLiteral(u"image");

    case ElementId::Name:
      return QStringLiteral(u"name");

    case ElementId::Id:
      return QStringLiteral(u"id");

    case ElementId::Width:
      return QStringLiteral(u"width");

    case ElementId::Height:
      return QStringLiteral(u"height");

    case ElementId::Visible:
      return QStringLiteral(u"visible");

    case ElementId::Opacity:
      return QStringLiteral(u"opacity");

    case ElementId::Type:
      return QStringLiteral(u"type");

    case ElementId::Value:
      return QStringLiteral(u"value");

    case ElementId::X:
      return QStringLiteral(u"x");

    case ElementId::Y:
      return QStringLiteral(u"y");

    case ElementId::Point:
      return QStringLiteral(u"point");

    default:
      throw TactileError{"Reached end of switch statement!"};
  }
}

}  // namespace tactile
