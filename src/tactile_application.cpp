#include "tactile_application.h"

#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "resize_dialog.h"
#include "settings_utils.h"
#include "tactile_core.h"
#include "tactile_window.h"
#include "tile_sheet_dialog.h"

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
  m_editor = std::make_unique<TactileCore>();

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
  using Core = TactileCore;

  auto* window = m_window.get();
  auto* editor = m_editor.get();

  connect(editor, &Core::s_updated, window, &W::trigger_redraw);

  connect(window, &W::s_added_row, editor, &Core::add_row);
  connect(window, &W::s_added_col, editor, &Core::add_col);
  connect(window, &W::s_removed_row, editor, &Core::remove_row);
  connect(window, &W::s_removed_col, editor, &Core::remove_col);

  connect(window, &W::s_new_map, editor, [editor](int id) noexcept {
    editor->new_map(id);
  });

  connect(window, &W::s_close_map, editor, &Core::close_map);
  connect(window, &W::s_select_map, editor, &Core::select_map);

  connect(window, &W::s_redraw, editor, &Core::draw);

  connect(window, &W::s_increase_tile_size, editor, &Core::increase_tile_size);
  connect(window, &W::s_decrease_tile_size, editor, &Core::decrease_tile_size);
  connect(window, &W::s_reset_tile_size, editor, &Core::reset_tile_size);

  connect(window, &W::s_pan_up, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(0, -(*tileSize));
    }
  });

  connect(window, &W::s_pan_right, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(*tileSize, 0);
    }
  });

  connect(window, &W::s_pan_down, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(0, *tileSize);
    }
  });

  connect(window, &W::s_pan_left, editor, [window, editor] {
    const auto tileSize = editor->tile_size();
    if (tileSize) {
      window->move_camera(-(*tileSize), 0);
    }
  });

  connect(window, &W::s_center_camera, this, [window, editor] {
    const auto width = editor->map_width();
    const auto height = editor->map_height();
    if (width && height) {
      window->center_camera(*width, *height);
    }
  });

  connect(window, &W::s_new_tile_sheet, this, [window, editor] {
    TileSheetDialog dialog;
    if (dialog.exec()) {
      const auto image = dialog.chosen_image();
      const auto tileWidth = dialog.chosen_width();
      const auto tileHeight = dialog.chosen_height();
      if (image && tileWidth && tileHeight) {
        const auto id = editor->add_tile_sheet(image, *tileWidth, *tileHeight);
        if (id) {
          window->add_tile_sheet(*id, image);
        }
      }
    }
  });

  connect(window, &W::s_resize_map, this, [window, editor] {
    ResizeDialog dialog;
    if (dialog.exec()) {
      editor->set_rows(*dialog.chosen_height());
      editor->set_cols(*dialog.chosen_width());
      window->trigger_redraw();
    }
  });
}

}  // namespace tactile