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
#include "tileset_dialog.h"

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
  using Window = TactileWindow;
  using Core = TactileCore;

  auto* window = m_window.get();
  auto* core = m_editor.get();

  connect(core, &Core::s_updated, window, &Window::trigger_redraw);

  window_to_core(&Window::s_added_row, &Core::add_row);
  window_to_core(&Window::s_added_row, &Core::add_row);
  window_to_core(&Window::s_added_col, &Core::add_col);
  window_to_core(&Window::s_removed_row, &Core::remove_row);
  window_to_core(&Window::s_removed_col, &Core::remove_col);

  window_to_core(&Window::s_new_map,
                 [core](int id) noexcept { core->new_map(id); });

  window_to_core(&Window::s_close_map, &Core::close_map);
  window_to_core(&Window::s_select_map, &Core::select_map);

  window_to_core(&Window::s_redraw, &Core::draw);

  window_to_core(&Window::s_increase_tile_size, &Core::increase_tile_size);
  window_to_core(&Window::s_decrease_tile_size, &Core::decrease_tile_size);
  window_to_core(&Window::s_reset_tile_size, &Core::reset_tile_size);

  window_to_core(&Window::s_pan_up, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->move_camera(0, *tileSize);
    }
  });

  window_to_core(&Window::s_pan_right, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->move_camera(-(*tileSize), 0);
    }
  });

  window_to_core(&Window::s_pan_down, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->move_camera(0, -(*tileSize));
    }
  });

  window_to_core(&Window::s_pan_left, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->move_camera(*tileSize, 0);
    }
  });

  window_to_this(&Window::s_center_camera, [window, core] {
    const auto width = core->map_width();
    const auto height = core->map_height();
    if (width && height) {
      window->center_camera(*width, *height);
    }
  });

  window_to_this(&Window::s_new_tileset, [window, core] {
    TilesetDialog dialog;
    if (dialog.exec()) {
      const auto image = dialog.chosen_image();
      const auto tileWidth = dialog.chosen_width();
      const auto tileHeight = dialog.chosen_height();
      if (image && tileWidth && tileHeight) {
        const auto id = core->add_tileset(image, *tileWidth, *tileHeight);
        if (id) {
          window->add_tileset(*id, image);
        }
      }
    }
  });

  window_to_this(&Window::s_resize_map, [window, core] {
    ResizeDialog dialog;
    if (dialog.exec()) {
      core->set_rows(*dialog.chosen_height());
      core->set_cols(*dialog.chosen_width());
      window->trigger_redraw();
    }
  });
}

}  // namespace tactile