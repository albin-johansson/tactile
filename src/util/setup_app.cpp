#include "setup_app.hpp"

#include <QApplication>
#include <QIcon>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "preferences.hpp"
#include "tactile_app.hpp"
#include "theme.hpp"
#include "version.hpp"

namespace tactile {
namespace {

/**
 * \brief Initialises the surface format used by the application.
 *
 * \since 0.1.0
 */
void init_surface_format() noexcept
{
  QSurfaceFormat format;
  format.setDepthBufferSize(0);
//  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
}

}  // namespace

void setup_app()
{
  init_surface_format();

  QApplication::setWindowIcon(QIcon{QStringLiteral(u":logo/windowIcon")});
  QApplication::setStyle(QStyleFactory::create(QStringLiteral(u"Fusion")));
  QApplication::setApplicationVersion(version());
  QApplication::setOrganizationName(QStringLiteral(u"albin-johansson"));
  QApplication::setApplicationName(QStringLiteral(u"tactile"));

  prefs::validate();

  if (const auto name = prefs::graphics::theme_name(); name) {
    theme::set_theme(*name);
  } else {
    theme::reset();
  }
}

}  // namespace tactile
