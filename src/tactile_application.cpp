#include "tactile_application.h"

#include <QFile>
#include <QIcon>
#include <QPainter>
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
  init_connections();

  m_window->showMaximized();
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

void TactileApplication::init_connections() noexcept
{
  using TW = TactileWindow;
  using TE = TactileEditor;

  auto* window = m_window.get();
  auto* editor = m_editor.get();

  connect(window, &TW::add_row, editor, &TE::add_row);
  connect(window, &TW::add_col, editor, &TE::add_col);
  connect(window, &TW::remove_row, editor, &TE::remove_row);
  connect(window, &TW::remove_col, editor, &TE::remove_col);

  connect(window, &TW::render, editor, &TE::draw);

  connect(editor, &TE::updated, window, &TW::trigger_redraw);
}

}  // namespace tactile