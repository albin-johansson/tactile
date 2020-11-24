#include "tmx_element.hpp"

#include <cassert>  // assert

#include "tactile_error.hpp"

namespace tactile::tmx {

tmx_element::tmx_element(const QDomElement& element) : m_element{element}
{}

auto tmx_element::contains(const element_id id) const -> bool
{
  return m_element.hasAttribute(stringify_element_id(id));
}

auto tmx_element::integer(const element_id id) const -> maybe<int>
{
  bool ok;
  const auto result = m_element.attribute(stringify_element_id(id)).toInt(&ok);
  if (ok) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto tmx_element::integer(const element_id id, const int def) const
    -> maybe<int>
{
  return integer(id).value_or(def);
}

auto tmx_element::floating(const element_id id, const double def) const
    -> double
{
  bool ok;
  const auto result =
      m_element.attribute(stringify_element_id(id)).toDouble(&ok);
  if (ok) {
    return result;
  } else {
    return def;
  }
}

auto tmx_element::string(const element_id id) const -> maybe<QString>
{
  const auto result = m_element.attribute(stringify_element_id(id));
  if (!result.isNull()) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto tmx_element::string(const element_id id, const QString& def) const
    -> QString
{
  return string(id).value_or(def);
}

auto tmx_element::stringify_element_id(const element_id type) -> QString
{
  switch (type) {
    case element_id::tileset:
      return QStringLiteral(u"tileset");

    case element_id::tilesets:
      return QStringLiteral(u"tilesets");

    case element_id::next_layer_id:
      return QStringLiteral(u"nextlayerid");

    case element_id::source:
      return QStringLiteral(u"source");

    case element_id::first_gid:
      return QStringLiteral(u"firstgid");

    case element_id::tile_width:
      return QStringLiteral(u"tilewidth");

    case element_id::tile_height:
      return QStringLiteral(u"tileheight");

    case element_id::image:
      return QStringLiteral(u"image");

    case element_id::name:
      return QStringLiteral(u"name");

    case element_id::id:
      return QStringLiteral(u"id");

    case element_id::width:
      return QStringLiteral(u"width");

    case element_id::height:
      return QStringLiteral(u"height");

    case element_id::visible:
      return QStringLiteral(u"visible");

    case element_id::opacity:
      return QStringLiteral(u"opacity");

    default:
      throw tactile_error{"Reached end of switch statement!"};
  }
}

}  // namespace tactile::tmx
