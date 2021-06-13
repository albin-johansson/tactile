#include "prepare_application.hpp"

#include <QApplication>    // QApplication
#include <QIcon>           // QIcon
#include <QStyleFactory>   // QStyleFactory
#include <QSurfaceFormat>  // QSurfaceFormat

#include "preferences.hpp"
#include "tactile_app.hpp"
#include "theme.hpp"
#include "version.hpp"

namespace tactile {
namespace {

void ConfigureSurfaceFormat()
{
  QSurfaceFormat format;
  format.setDepthBufferSize(0);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
}

}  // namespace

void PrepareApplication()
{
  ConfigureSurfaceFormat();

  QApplication::setWindowIcon(QIcon{QStringLiteral(u":logo/windowIcon")});
  QApplication::setStyle(QStyleFactory::create(QStringLiteral(u"Fusion")));
  QApplication::setApplicationVersion(Version());
  QApplication::setOrganizationName(QStringLiteral(u"albin-johansson"));
  QApplication::setApplicationName(QStringLiteral(u"tactile"));

  prefs::ValidatePreferences();
  ValidateThemes();

  if (const auto name = prefs::ThemeName())
  {
    SetTheme(*name);
  }
  else
  {
    ResetTheme();
  }
}

}  // namespace tactile
