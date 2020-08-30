#include "parse_palette.hpp"

#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>

#include "tactile_error.hpp"

namespace tactile::json {
namespace {

void parse_group(const QJsonObject& json,
                 QPalette& palette,
                 QPalette::ColorGroup group = QPalette::ColorGroup::All)
{
  using color_role = QPalette::ColorRole;

  const auto set_if_exists = [&](color_role role, u16_czstring key) {
    if (const auto it = json.find(key); it != json.end()) {
      const auto value = it.value();
      Q_ASSERT(value.isString());

      palette.setColor(group, role, QColor{value.toString()});
    }
  };

  set_if_exists(color_role::Window, u"Window");
  set_if_exists(color_role::WindowText, u"WindowText");

  set_if_exists(color_role::Base, u"Base");
  set_if_exists(color_role::AlternateBase, u"AlternateBase");

  set_if_exists(color_role::ToolTipBase, u"ToolTipBase");
  set_if_exists(color_role::ToolTipText, u"ToolTipText");

  set_if_exists(color_role::Text, u"Text");
  set_if_exists(color_role::BrightText, u"BrightText");

  set_if_exists(color_role::Button, u"Button");
  set_if_exists(color_role::ButtonText, u"ButtonText");

  set_if_exists(color_role::Light, u"Light");
  set_if_exists(color_role::Midlight, u"Midlight");
  set_if_exists(color_role::Dark, u"Dark");
  set_if_exists(color_role::Mid, u"Mid");
  set_if_exists(color_role::Shadow, u"Shadow");

  set_if_exists(color_role::Highlight, u"Highlight");
  set_if_exists(color_role::HighlightedText, u"HighlightedText");

  set_if_exists(color_role::Link, u"Link");
  set_if_exists(color_role::LinkVisited, u"LinkVisited");
}

[[nodiscard]] auto get_json_object(const QString& file) -> QJsonObject
{
  QFile themeFile{file};

  if (!themeFile.exists()) {
    throw tactile_error{"File doesn't exist!"};
  }

  themeFile.open(QFile::ReadOnly | QFile::Text);

  QJsonParseError error{};
  const auto document = QJsonDocument::fromJson(themeFile.readAll(), &error);
  if (document.isNull()) {
    throw tactile_error{"Couldn't open/find JSON theme file: " +
                        error.errorString().toStdString()};
  }

  Q_ASSERT(document.isObject());
  return document.object();
}

}  // namespace

auto parse_palette(const QString& file) -> QPalette
{
  QPalette palette;

  const auto json = get_json_object(file);

  parse_group(json, palette);

  if (constexpr auto disabled = u"disabled"; json.contains(disabled)) {
    const auto item = json[disabled];
    Q_ASSERT(item.isObject());

    const auto obj = item.toObject();
    parse_group(obj, palette, QPalette::ColorGroup::Disabled);
  }

  return palette;
}

}  // namespace tactile::json
