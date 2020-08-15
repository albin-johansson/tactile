#include "setup_app.hpp"

#include <QApplication>
#include <QGuiApplication>
#include <QIcon>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "app.hpp"
#include "setting.hpp"
#include "setting_identifiers.hpp"
#include "version.hpp"

namespace tactile {
namespace {

/**
 * @brief Initialises the surface format used by the application.
 *
 * @since 0.1.0
 */
void init_surface_format() noexcept
{
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
}

/**
 * @brief Initializes the settings to their default values, if they are absent.
 *
 * @since 0.1.0
 */
void validate_settings() noexcept
{
  settings::set_if_missing(cfg::graphics::grid(), true);
}

}  // namespace

void setup_app()
{
  init_surface_format();

  QGuiApplication::setWindowIcon(QIcon{":/resources/icons/tactile2_icon2.png"});

  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication::setApplicationVersion(version());
  QApplication::setOrganizationName("albin-johansson");
  QApplication::setApplicationName("tactile");

  validate_settings();
}

}  // namespace tactile
