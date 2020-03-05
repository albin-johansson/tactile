#include "tactile_application.h"

#include <QFile>
#include <QIcon>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "tactile_editor.h"
#include "tactile_window.h"

namespace tactile {
namespace {

/**
 * Initialises the surface format used by the application.
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

}  // namespace

TactileApplication::TactileApplication(int argc, char** argv)
    : QApplication{argc, argv},
      m_window{std::make_unique<TactileWindow>()},
      m_editor{std::make_unique<TactileEditor>()}
{
  init_surface_format();

  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setApplicationName("Tactile");

  QIcon icon{":/res/icons/tactile2_icon2.png"};
  setWindowIcon(icon);

  load_style_sheet(":/res/tactile_light.qss");

  m_window->showMaximized();

  // TODO connect window paint events to editor paint
}

TactileApplication::~TactileApplication() noexcept = default;

void TactileApplication::load_style_sheet(const char* styleSheet)
{
  if (!styleSheet) {
    return;
  }

  QFile file{styleSheet};
  file.open(QFile::ReadOnly);
  QString StyleSheet = QLatin1String(file.readAll());
  setStyleSheet(StyleSheet);
}

}  // namespace tactile