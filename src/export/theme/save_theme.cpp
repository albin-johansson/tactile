#include "save_theme.hpp"

#include <QJsonDocument>  // QJsonDocument
#include <QJsonObject>    // QJsonObject

#include "json_utils.hpp"

namespace tactile {
namespace {

void SaveGroup(QJsonObject& object,
               const QPalette& theme,
               const QPalette::ColorGroup group)
{
  const auto save = [&](const QStringView key, const QPalette::ColorRole role) {
    object.insert(key, theme.color(group, role).name(QColor::HexRgb));
  };

  save(u"Window", QPalette::Window);
  save(u"Base", QPalette::Base);
  save(u"AlternateBase", QPalette::AlternateBase);
  save(u"Text", QPalette::Text);
  save(u"BrightText", QPalette::BrightText);
  save(u"WindowText", QPalette::WindowText);
  save(u"ButtonText", QPalette::ButtonText);
  save(u"ToolTipBase", QPalette::ToolTipBase);
  save(u"ToolTipText", QPalette::ToolTipText);
  save(u"Button", QPalette::Button);
  save(u"Link", QPalette::Link);
  save(u"LinkVisited", QPalette::LinkVisited);
  save(u"Highlight", QPalette::Highlight);
  save(u"HighlightedText", QPalette::HighlightedText);
  save(u"Dark", QPalette::Dark);
  save(u"Light", QPalette::Light);
}

[[nodiscard]] auto CreateThemeRoot(const QPalette& theme) -> QJsonObject
{
  QJsonObject root;
  SaveGroup(root, theme, QPalette::Active);

  QJsonObject disabled;
  SaveGroup(disabled, theme, QPalette::Disabled);

  root.insert(u"disabled", disabled);

  return root;
}

}  // namespace

void SaveTheme(const QString& path, const QPalette& theme)
{
  QJsonDocument document{};

  document.setObject(CreateThemeRoot(theme));
  WriteJson(QFileInfo{path}, document);
}

}  // namespace tactile
