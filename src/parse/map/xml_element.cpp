#include "xml_element.hpp"

#include <cassert>  // assert

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::parse {

xml_element::xml_element(const QDomElement& element) : m_element{element}
{}

auto xml_element::contains(const element_id id) const -> bool
{
  return m_element.hasAttribute(stringify_element_id(id));
}

auto xml_element::integer(const element_id id) const -> maybe<int>
{
  bool ok;
  const auto result = m_element.attribute(stringify_element_id(id)).toInt(&ok);
  if (ok) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto xml_element::integer(const element_id id, const int def) const
    -> maybe<int>
{
  return integer(id).value_or(def);
}

auto xml_element::floating(const element_id id) const -> maybe<double>
{
  bool ok;
  const auto result =
      m_element.attribute(stringify_element_id(id)).toDouble(&ok);
  if (ok) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto xml_element::floating(const element_id id, const double def) const
    -> double
{
  return floating(id).value_or(def);
}

auto xml_element::string(const element_id id) const -> maybe<QString>
{
  const auto result = m_element.attribute(stringify_element_id(id));
  if (!result.isNull()) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto xml_element::string(const element_id id, const QString& def) const
    -> QString
{
  return string(id).value_or(def);
}

auto xml_element::boolean(const element_id id) const -> maybe<bool>
{
  if (const auto value = string(id)) {
    return value == TACTILE_QSTRING(u"true");
  } else {
    return std::nullopt;
  }
}

auto xml_element::stringify_element_id(const element_id type) -> QString
{
  switch (type) {
    case element_id::tileset:
      return TACTILE_QSTRING(u"tileset");

    case element_id::tilesets:
      throw tactile_error{"TMX format does not feature \"tilesets\"!"};

    case element_id::next_layer_id:
      return TACTILE_QSTRING(u"nextlayerid");

    case element_id::source:
      return TACTILE_QSTRING(u"source");

    case element_id::first_gid:
      return TACTILE_QSTRING(u"firstgid");

    case element_id::tile_width:
      return TACTILE_QSTRING(u"tilewidth");

    case element_id::tile_height:
      return TACTILE_QSTRING(u"tileheight");

    case element_id::image:
      return TACTILE_QSTRING(u"image");

    case element_id::name:
      return TACTILE_QSTRING(u"name");

    case element_id::id:
      return TACTILE_QSTRING(u"id");

    case element_id::width:
      return TACTILE_QSTRING(u"width");

    case element_id::height:
      return TACTILE_QSTRING(u"height");

    case element_id::visible:
      return TACTILE_QSTRING(u"visible");

    case element_id::opacity:
      return TACTILE_QSTRING(u"opacity");

    case element_id::type:
      return TACTILE_QSTRING(u"type");

    case element_id::value:
      return TACTILE_QSTRING(u"value");

    default:
      throw tactile_error{"Reached end of switch statement!"};
  }
}

}  // namespace tactile::parse
