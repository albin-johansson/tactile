#include "theme.hpp"

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringView>

#include "setting.hpp"
#include "setting_identifiers.hpp"
#include "tactile_error.hpp"
#include "tactile_types.hpp"

namespace tactile::prefs::theme {
namespace {

inline constexpr QStringView light{u"Light"};
inline constexpr QStringView dark{u"Dark"};
inline constexpr QStringView atomOneDark{u"Atom One Dark"};

[[nodiscard]] auto is_standard_theme(QStringView name) -> bool
{
  if (name.isNull() || name.isEmpty()) {
    return false;
  }

  return light == name || dark == name || atomOneDark == name;
}

class palette
{
 public:
  using role = QPalette::ColorRole;
  using group = QPalette::ColorGroup;

  explicit palette(const QJsonObject& json)
  {
    set(json);

    if (json.contains(u"disabled")) {
      const auto item = json[u"disabled"];
      Q_ASSERT(item.isObject());

      const auto obj = item.toObject();
      set(obj, group::Disabled);
    }
  }

  [[nodiscard]] auto get() const -> const QPalette& { return m_palette; }

 private:
  QPalette m_palette;

  void set_if_exists(const QJsonObject& json,
                     group group,
                     role role,
                     u16_czstring key)
  {
    if (const auto iterator = json.find(key); iterator != json.end()) {
      const auto value = iterator.value();
      Q_ASSERT(value.isString());

      m_palette.setColor(group, role, QColor{value.toString()});
    } else {
      qDebug() << "Did not find key in theme file:" << QStringView{key};
    }
  }

  void set(const QJsonObject& json, group group = group::All)
  {
    set_if_exists(json, group, role::Window, u"Window");
    set_if_exists(json, group, role::WindowText, u"WindowText");

    set_if_exists(json, group, role::Base, u"Base");
    set_if_exists(json, group, role::AlternateBase, u"AlternateBase");

    set_if_exists(json, group, role::ToolTipBase, u"ToolTipBase");
    set_if_exists(json, group, role::ToolTipText, u"ToolTipText");

    set_if_exists(json, group, role::Text, u"Text");
    set_if_exists(json, group, role::BrightText, u"BrightText");

    set_if_exists(json, group, role::Button, u"Button");
    set_if_exists(json, group, role::ButtonText, u"ButtonText");

    set_if_exists(json, group, role::Light, u"Light");
    set_if_exists(json, group, role::Midlight, u"Midlight");
    set_if_exists(json, group, role::Dark, u"Dark");
    set_if_exists(json, group, role::Mid, u"Mid");
    set_if_exists(json, group, role::Shadow, u"Shadow");

    set_if_exists(json, group, role::Highlight, u"Highlight");
    set_if_exists(json, group, role::HighlightedText, u"HighlightedText");

    set_if_exists(json, group, role::Link, u"Link");
    set_if_exists(json, group, role::LinkVisited, u"LinkVisited");
  }
};

}  // namespace

void reset()
{
  set_theme(dark.toString());
}

void set_theme(const QString& name)
{
  if (is_standard_theme(name)) {
    const auto palette = from_name(name);

    prefs::set(id::graphics::theme(), palette);
    prefs::set(id::graphics::theme_name(), name);
    QApplication::setPalette(palette);

  } else {
    // parse

    throw tactile_error{"Not supported yet..."};
  }
}

auto from_name(const QString& name) -> QPalette
{
  if (dark == name) {
    return get_dark();
  } else if (atomOneDark == name) {
    return get_atom_one_dark();
  } else if (light == name) {
    return get_light();
  } else {
    throw tactile_error{"Couldn't obtain palette from name!"};
  }
}

auto parse(const QByteArray& name) -> QPalette
{
  QFile themeFile{name};
  themeFile.open(QFile::ReadOnly | QFile::Text);

  QJsonParseError error{};
  const auto document = QJsonDocument::fromJson(themeFile.readAll(), &error);
  if (document.isNull()) {
    throw tactile_error{"Couldn't open/find JSON theme file: " +
                        error.errorString().toStdString()};
  }

  Q_ASSERT(document.isObject());
  palette palette{document.object()};

  return palette.get();
}

auto get_dark() -> const QPalette&
{
  static const auto dark = parse(":theme/dark");
  return dark;
}

auto get_light() -> const QPalette&
{
  static const auto light = parse(":theme/light");
  return light;
}

auto get_atom_one_dark() -> const QPalette&
{
  static const auto oneDark = parse(":theme/atomOneDark");
  return oneDark;
}

auto get_default() -> const QPalette&
{
  return get_dark();
}

auto get_default_name() -> QStringView
{
  return dark;
}

}  // namespace tactile::prefs::theme
