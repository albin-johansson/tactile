#include "prepare_application.hpp"

#include <QApplication>    // QApplication
#include <QIcon>           // QIcon
#include <QStyleFactory>   // QStyleFactory
#include <QSurfaceFormat>  // QSurfaceFormat

#include "preferences.hpp"
#include "tactile_app.hpp"
#include "tactile_qstring.hpp"
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

  QApplication::setWindowIcon(QIcon{TACTILE_QSTRING(u":logo/windowIcon")});
  QApplication::setStyle(QStyleFactory::create(TACTILE_QSTRING(u"Fusion")));
  QApplication::setApplicationVersion(version());
  QApplication::setOrganizationName(TACTILE_QSTRING(u"albin-johansson"));
  QApplication::setApplicationName(TACTILE_QSTRING(u"tactile"));

  prefs::ValidatePreferences();
  validate_themes();

  if (const auto name = prefs::ThemeName())
  {
    set_theme(*name);
  }
  else
  {
    reset_theme();
  }
}

}  // namespace tactile
