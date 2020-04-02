#include "tactile_application.h"

#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "resize_dialog.h"
#include "settings_utils.h"
#include "tactile_editor.h"
#include "tactile_window.h"
#include "tile_sheet_dialog.h"
#include "tile_sheet_file_dialog.h"
#include "tile_size.h"

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

/**
 * Initializes the settings to their default values, if they are absent.
 *
 * @since 0.1.0
 */
void validate_settings() noexcept
{
  set_if_absent("visuals-grid", true);
  set_if_absent("load-previous-layout-on-startup", true);
}

}  // namespace

TactileApplication::TactileApplication(int argc, char** argv)
    : QApplication{argc, argv}
{
  init_surface_format();

  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setOrganizationName("albinjohansson");
  setApplicationName("Tactile");
  //  setOrganizationDomain("https://github.com/albin-johansson/Tactile");

  QIcon icon{":/res/icons/tactile2_icon2.png"};
  setWindowIcon(icon);

  validate_settings();

  m_window = std::make_unique<TactileWindow>();
  m_editor = std::make_unique<TactileEditor>();

  init_connections();
  load_style_sheet(":/res/tactile_light.qss");

  m_window->show();
}

TactileApplication::~TactileApplication() noexcept = default;

void TactileApplication::load_style_sheet(const char* styleSheet)
{
  if (!styleSheet) {
    return;
  }

  QFile file{styleSheet};
  file.open(QFile::ReadOnly);

  auto style = QLatin1String{file.readAll()};
  setStyleSheet(style);
}

void TactileApplication::init_connections() noexcept
{
  using W = TactileWindow;
  using E = TactileEditor;

  auto* window = m_window.get();
  auto* editor = m_editor.get();

  connect(editor, &E::updated, window, &W::trigger_redraw);

  connect(window, &W::tw_added_row, editor, &E::add_row);
  connect(window, &W::tw_added_col, editor, &E::add_col);
  connect(window, &W::tw_removed_row, editor, &E::remove_row);
  connect(window, &W::tw_removed_col, editor, &E::remove_col);

  connect(window, &W::tw_new_map, editor, [window, editor](int id) {
    editor->new_map(id);
    // TODO
  });

  connect(window, &W::tw_close_map, editor, &E::close_map);
  connect(window, &W::tw_select_map, editor, &E::select_map);

  connect(window, &W::tw_render, editor, &E::draw);

  connect(window, &W::tw_increase_tile_size, editor, &E::increase_tile_size);
  connect(window, &W::tw_decrease_tile_size, editor, &E::decrease_tile_size);
  connect(window, &W::tw_reset_tile_size, editor, &E::reset_tile_size);

  connect(window, &W::tw_pan_up, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(0, -(*tileSize));
    }
  });

  connect(window, &W::tw_pan_right, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(*tileSize, 0);
    }
  });

  connect(window, &W::tw_pan_down, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(0, *tileSize);
    }
  });

  connect(window, &W::tw_pan_left, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(-(*tileSize), 0);
    }
  });

  connect(window, &W::tw_center_camera, this, [window, editor] {
    const auto width = editor->map_width();
    const auto height = editor->map_height();
    if (width && height) {
      window->center_camera(*width, *height);
    }
  });

  connect(window, &W::tw_new_tile_sheet, this, [window, editor] {
    TileSheetDialog dialog;
    if (dialog.exec()) {
      const auto image = dialog.chosen_image();
      const auto tileWidth = dialog.chosen_width();
      const auto tileHeight = dialog.chosen_height();
      if (image && tileWidth && tileHeight) {
        const auto id = editor->add_tile_sheet(image, *tileWidth, *tileHeight);
        if (id) {
          window->add_tile_sheet(*id);
        }
      }
    }
  });

  connect(window, &W::tw_resize_map, this, [window, editor] {
    ResizeDialog dialog;
    if (dialog.exec()) {
      editor->set_rows(*dialog.chosen_height());
      editor->set_cols(*dialog.chosen_width());
      window->trigger_redraw();
    }
  });
}

}  // namespace tactile