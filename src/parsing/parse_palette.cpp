#include "parse_palette.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "file_handle.hpp"
#include "tactile_error.hpp"

namespace tactile {
namespace {

void ParseGroup(const QJsonObject& json,
                QPalette& palette,
                const QPalette::ColorGroup group = QPalette::ColorGroup::All)
{
  const auto setIfExists = [&](const QPalette::ColorRole role, cz16string key) {
    if (const auto it = json.find(key); it != json.end())
    {
      const auto value = it.value();
      Q_ASSERT(value.isString());

      palette.setColor(group, role, QColor{value.toString()});
    }
  };

  setIfExists(QPalette::ColorRole::Window, u"Window");
  setIfExists(QPalette::ColorRole::WindowText, u"WindowText");

  setIfExists(QPalette::ColorRole::Base, u"Base");
  setIfExists(QPalette::ColorRole::AlternateBase, u"AlternateBase");

  setIfExists(QPalette::ColorRole::ToolTipBase, u"ToolTipBase");
  setIfExists(QPalette::ColorRole::ToolTipText, u"ToolTipText");

  setIfExists(QPalette::ColorRole::Text, u"Text");
  setIfExists(QPalette::ColorRole::BrightText, u"BrightText");

  setIfExists(QPalette::ColorRole::Button, u"Button");
  setIfExists(QPalette::ColorRole::ButtonText, u"ButtonText");

  setIfExists(QPalette::ColorRole::Light, u"Light");
  setIfExists(QPalette::ColorRole::Midlight, u"Midlight");
  setIfExists(QPalette::ColorRole::Dark, u"Dark");
  setIfExists(QPalette::ColorRole::Mid, u"Mid");
  setIfExists(QPalette::ColorRole::Shadow, u"Shadow");

  setIfExists(QPalette::ColorRole::Highlight, u"Highlight");
  setIfExists(QPalette::ColorRole::HighlightedText, u"HighlightedText");

  setIfExists(QPalette::ColorRole::Link, u"Link");
  setIfExists(QPalette::ColorRole::LinkVisited, u"LinkVisited");

  setIfExists(QPalette::PlaceholderText, u"PlaceholderText");
}

[[nodiscard]] auto GetJsonObject(const QString& file) -> Maybe<QJsonObject>
{
  FileHandle themeFile{file};
  if (!themeFile.Exists())
  {
    return nothing;
  }

  themeFile.Open(QFile::ReadOnly | QFile::Text);

  const auto document = QJsonDocument::fromJson(themeFile.Read());
  if (document.isNull())
  {
    return nothing;
  }

  Q_ASSERT(document.isObject());
  return document.object();
}

}  // namespace

auto ParsePalette(const QString& file) -> Maybe<QPalette>
{
  QPalette palette;

  const auto json = GetJsonObject(file);
  if (!json)
  {
    return nothing;
  }

  ParseGroup(*json, palette);

  if (const auto it = json->find(u"disabled"); it != json->end())
  {
    const auto item = it.value();
    Q_ASSERT(item.isObject());

    const auto obj = item.toObject();
    ParseGroup(obj, palette, QPalette::ColorGroup::Disabled);
  }

  return palette;
}

}  // namespace tactile
