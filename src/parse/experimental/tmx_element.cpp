#include "tmx_element.hpp"

#include <cassert>  // assert

namespace tactile::tmx {

tmx_element::tmx_element(const QDomElement& element) : m_element{element}
{}

auto tmx_element::contains(const QString& str) const -> bool
{
  return m_element.hasAttribute(str);
}

auto tmx_element::integer(const QString& str) const -> maybe<int>
{
  bool ok;
  if (const auto result = m_element.attribute(str).toInt(&ok); ok) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto tmx_element::integer(const element_type id) const -> maybe<int>
{
  return integer(stringify_element_id(id));
}

auto tmx_element::integer(const QString& str, const int def) const -> int
{
  return integer(str).value_or(def);
}

auto tmx_element::floating(const QString& str, const double def) const -> double
{
  bool ok;
  if (const auto result = m_element.attribute(str).toDouble(&ok); ok) {
    return result;
  } else {
    return def;
  }
}

auto tmx_element::string(const QString& str) const -> maybe<QString>
{
  const auto result = m_element.attribute(str);
  if (!result.isNull()) {
    return result;
  } else {
    return std::nullopt;
  }
}

auto tmx_element::string(const QString& str, const QString& def) const
    -> QString
{
  return string(str).value_or(def);
}

auto tmx_element::stringify_element_id(const element_type type) -> QString
{
  switch (type) {
    case element_type::tileset:
      return QStringLiteral(u"tileset");

    case element_type::tilesets:
      return QStringLiteral(u"tilesets");

    case element_type::next_layer_id:
      return QStringLiteral(u"nextlayerid");

    case element_type::source:
      return QStringLiteral(u"source");

    case element_type::first_gid:
      return QStringLiteral(u"firstgid");

    case element_type::tile_width:
      return QStringLiteral(u"tilewidth");

    case element_type::tile_height:
      return QStringLiteral(u"tileheight");

    case element_type::image:
      return QStringLiteral(u"image");

    case element_type::name:
      return QStringLiteral(u"name");

    case element_type::id:
      return QStringLiteral(u"id");

    case element_type::width:
      return QStringLiteral(u"width");

    case element_type::height:
      return QStringLiteral(u"height");

    case element_type::visible:
      return QStringLiteral(u"visible");

    case element_type::opacity:
      return QStringLiteral(u"opacity");

    default:
      assert(false);
  }
}

}  // namespace tactile::tmx
