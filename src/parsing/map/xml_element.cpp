#include "xml_element.hpp"

#include <cassert>  // assert

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::parse {

xml_element::xml_element(const QDomElement& element) : m_element{element}
{}

auto xml_element::contains(const ElementId id) const -> bool
{
  return m_element.hasAttribute(stringify_element_id(id));
}

auto xml_element::integer(const ElementId id) const -> maybe<int>
{
  bool ok;
  const auto result = m_element.attribute(stringify_element_id(id)).toInt(&ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto xml_element::integer(const ElementId id, const int def) const
    -> maybe<int>
{
  return integer(id).value_or(def);
}

auto xml_element::floating(const ElementId id) const -> maybe<double>
{
  bool ok;
  const auto result =
      m_element.attribute(stringify_element_id(id)).toDouble(&ok);
  if (ok)
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto xml_element::floating(const ElementId id, const double def) const
    -> double
{
  return floating(id).value_or(def);
}

auto xml_element::string(const ElementId id) const -> maybe<QString>
{
  const auto result = m_element.attribute(stringify_element_id(id));
  if (!result.isNull())
  {
    return result;
  }
  else
  {
    return std::nullopt;
  }
}

auto xml_element::string(const ElementId id, const QString& def) const
    -> QString
{
  return string(id).value_or(def);
}

auto xml_element::boolean(const ElementId id) const -> maybe<bool>
{
  if (const auto value = string(id))
  {
    return value == TACTILE_QSTRING(u"true");
  }
  else
  {
    return std::nullopt;
  }
}

auto xml_element::stringify_element_id(const ElementId type) -> QString
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
