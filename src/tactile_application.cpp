#include "tactile_application.h"

#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QStyleFactory>
#include <QSurfaceFormat>
#include <iostream>

#include "tactile_editor.h"
#include "tactile_window.h"
#include "tile_sheet_file_dialog.h"

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
  using W = TactileWindow;
  using E = TactileEditor;

  auto* window = m_window.get();
  auto* editor = m_editor.get();

  connect(window, &W::req_add_row, editor, &E::add_row);
  connect(window, &W::req_add_col, editor, &E::add_col);

  connect(window, &W::req_remove_row, editor, &E::remove_row);
  connect(window, &W::req_remove_col, editor, &E::remove_col);

  connect(window, &W::req_center_camera, this, [window, editor] {
    const auto width = editor->cols() * 50;  // FIXME add tile size
    const auto height = editor->rows() * 50;
    window->center_camera(width, height);
  });

  connect(window, &W::req_new_tile_sheet, this, [window, editor] {
    const auto result = open_tile_sheet_image(window);
    if (result) {
      editor->add_tile_sheet(result->toStdString().c_str());
    }
  });

  connect(window, &W::req_render, editor, &E::draw);

  connect(editor, &E::updated, window, &W::trigger_redraw);
}

}  // namespace tactile