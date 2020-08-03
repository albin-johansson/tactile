#include "window.hpp"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QSettings>
#include <QSpacerItem>

#include "about_dialog.hpp"
#include "central_editor_widget.hpp"
#include "create_dock_widget.hpp"
#include "mouse_tool_widget.hpp"
#include "settings_dialog.hpp"
#include "settings_utils.hpp"
#include "tileset_widget.hpp"
#include "ui_window.h"
#include "widget_size_policy.hpp"

namespace tactile::ui {

window::window(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

  // TODO add mini-map widget

  m_centralWidget = new central_editor_widget{};
  m_mouseToolWidget = new mouse_tool_widget{};
  m_tilesetWidget = new tileset_widget{};

  setCentralWidget(m_centralWidget);  // claims ownership of central widget

  //  centralWidget()->layout()->setContentsMargins(0, 2, 0, 0);

  m_mouseToolDock = create_dock_widget(m_mouseToolWidget, "mouseToolDock");
  set_size_policy(
      m_mouseToolDock.get(), QSizePolicy::Minimum, QSizePolicy::Expanding);
  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_mouseToolDock.get());

  m_tilesetDock = create_dock_widget(m_tilesetWidget, "tilesetDock");
  m_tilesetDock->setWindowTitle("Tilesets");

  set_size_policy(m_tilesetDock.get(), QSizePolicy::Expanding);
  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock.get());

  init_connections();
  init_layout();
  enable_startup_view();  // TODO option to reopen last tilemap
}

window::~window() noexcept
{
  delete m_ui;
}

auto window::unique(QWidget* parent) -> std::unique_ptr<window>
{
  return std::make_unique<window>(parent);
}

void window::handle_undo_state_update(bool canUndo)
{
  m_ui->actionUndo->setEnabled(canUndo);
}

void window::handle_redo_state_update(bool canUndo)
{
  m_ui->actionRedo->setEnabled(canUndo);
}

// TODO these delegations can be replaced with signals

void window::handle_add_tileset(const model::tileset_info& info,
                                const QString& tabName) noexcept
{
  m_tilesetWidget->add_tileset(info, tabName);
}

void window::handle_remove_tileset(int id) noexcept
{
  m_tilesetWidget->remove_tileset(id);
}

void window::enable_startup_view() noexcept
{
  m_centralWidget->enable_startup_view();
  m_mouseToolWidget->disable_tools();
  hide_all_docks();
}

void window::enable_editor_view() noexcept
{
  m_centralWidget->enable_editor_view();
  m_mouseToolWidget->enable_tools();  // FIXME only enable relevant tools
}

void window::handle_display_about_dialog() noexcept
{
  about_dialog about;
  about.exec();
}

void window::handle_display_settings_dialog() noexcept
{
  settings_dialog settings;
  settings.exec();
}

void window::handle_center_camera(int mapWidth, int mapHeight)
{
  m_centralWidget->center_viewport(mapWidth, mapHeight);
  handle_draw();
}

void window::handle_move_camera(int dx, int dy)
{
  m_centralWidget->move_viewport(dx, dy);
  handle_draw();
}

void window::handle_draw()
{
  m_centralWidget->handle_trigger_redraw();
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);

  QSettings settings;
  settings.setValue("last-open-layout-state", saveState());
  settings.setValue("last-open-layout-geometry", saveGeometry());
}

void window::init_connections() noexcept
{
  connect(m_centralWidget,
          &central_editor_widget::request_remove_tab,
          this,
          [this](int id) noexcept {
            emit request_close_map(id);

            // FIXME borderline hack, the tab isn't actually removed yet so 1
            //  is the same as checking if there are not open tabs.
            if (m_centralWidget->open_tabs() == 1) {
              enable_startup_view();
            }
          });

  on_triggered(m_ui->actionExit, [] { QApplication::exit(); });

  on_triggered(m_ui->actionNewMap, [this]() noexcept {
    const auto id = m_centralWidget->add_new_map_tab("map");

    emit request_new_map(id);

    if (!in_editor_mode()) {
      enable_editor_view();
      show_all_docks();  // TODO just reopen docks that were visible
      emit request_center_camera();
    }

    // TODO...
  });

  on_triggered(m_ui->actionCloseMap, [this]() noexcept {
    if (in_editor_mode()) {
      // TODO save current state of open map

      const auto id = m_centralWidget->active_tab_id();
      if (id) {
        m_centralWidget->close_tab(*id);
        emit request_close_map(*id);

        if (m_centralWidget->open_tabs() == 0) {
          enable_startup_view();
        }
      } else {
        qWarning("Failed to close map, no active tab ID!");
      }
    }
  });

  using W = window;

  on_triggered(m_ui->actionAboutTactile, &W::handle_display_about_dialog);
  on_triggered(m_ui->actionSettings, &W::handle_display_settings_dialog);

  on_triggered(m_ui->actionUndo, [this] {
    if (in_editor_mode()) {
      emit request_undo();
    }
  });

  on_triggered(m_ui->actionRedo, [this] {
    if (in_editor_mode()) {
      emit request_redo();
    }
  });

  // TODO look into making listeners of signals check if window is in editor
  //  mode?

  on_triggered(m_ui->actionAddTileset, &W::request_new_tileset);

  on_triggered(m_ui->actionAddRow, [this]() noexcept {
    if (in_editor_mode()) {
      emit request_add_row();
    }
  });

  on_triggered(m_ui->actionAddColumn, [this]() noexcept {
    if (in_editor_mode()) {
      emit request_add_col();
    }
  });

  on_triggered(m_ui->actionRemoveRow, [this]() noexcept {
    if (in_editor_mode()) {
      emit request_remove_row();
    }
  });

  on_triggered(m_ui->actionRemoveColumn, [this]() noexcept {
    if (in_editor_mode()) {
      emit request_remove_col();
    }
  });

  on_triggered(m_ui->actionToggleGrid, [this] {
    if (in_editor_mode()) {
      QSettings settings;
      const auto prev = settings.value("visuals-grid").toBool();
      settings.setValue("visuals-grid", !prev);
      handle_draw();
    }
  });

  on_triggered(m_ui->actionZoomIn, [this] {
    if (in_editor_mode()) {
      emit request_increase_tile_size();
    }
  });

  on_triggered(m_ui->actionZoomOut, [this] {
    if (in_editor_mode()) {
      emit request_decrease_tile_size();
    }
  });

  on_triggered(m_ui->actionResetZoom, [this] {
    if (in_editor_mode()) {
      emit request_reset_tile_size();
    }
  });

  on_triggered(m_ui->actionPanUp, [this] {
    if (in_editor_mode()) {
      emit request_pan_up();
    }
  });

  on_triggered(m_ui->actionPanDown, [this] {
    if (in_editor_mode()) {
      emit request_pan_down();
    }
  });

  on_triggered(m_ui->actionPanRight, [this] {
    if (in_editor_mode()) {
      emit request_pan_right();
    }
  });

  on_triggered(m_ui->actionPanLeft, [this] {
    if (in_editor_mode()) {
      emit request_pan_left();
    }
  });

  on_triggered(m_ui->actionCenterCamera, [this] {
    if (in_editor_mode()) {
      emit request_center_camera();
    }
  });

  on_triggered(m_ui->actionResizeMap, [this] {
    if (in_editor_mode()) {
      emit request_resize_map();
    }
  });

  on_triggered(m_ui->actionMouseTools, [this] {
    m_mouseToolDock->setVisible(m_ui->actionMouseTools->isChecked());
  });

  on_triggered(m_ui->actionTilesets, [this] {
    m_tilesetDock->setVisible(m_ui->actionTilesets->isChecked());
  });

  on_triggered(m_ui->actionResetLayout, [this] { reset_dock_layout(); });

  {
    m_mouseToolGroup = std::make_unique<QActionGroup>(this);
    m_mouseToolGroup->setExclusive(true);
    m_mouseToolGroup->addAction(m_ui->actionStampTool);
    m_mouseToolGroup->addAction(m_ui->actionBucketTool);
    m_mouseToolGroup->addAction(m_ui->actionFindSameTool);
    m_mouseToolGroup->addAction(m_ui->actionEraserTool);
    m_mouseToolGroup->addAction(m_ui->actionRectangleTool);

    using MTW = mouse_tool_widget;

    connect(m_mouseToolWidget, &MTW::s_stamp_enabled, this, [this] {
      m_ui->actionStampTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::s_bucket_enabled, this, [this] {
      m_ui->actionBucketTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::s_eraser_enabled, this, [this] {
      m_ui->actionEraserTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::s_rectangle_enabled, this, [this] {
      m_ui->actionRectangleTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::s_find_same_enabled, this, [this] {
      m_ui->actionFindSameTool->setChecked(true);
    });

    on_triggered(m_ui->actionStampTool,
                 [this] { m_mouseToolWidget->handle_enable_stamp(); });

    on_triggered(m_ui->actionBucketTool,
                 [this] { m_mouseToolWidget->handle_enable_bucket(); });

    on_triggered(m_ui->actionEraserTool,
                 [this] { m_mouseToolWidget->handle_enable_eraser(); });

    on_triggered(m_ui->actionRectangleTool,
                 [this] { m_mouseToolWidget->handle_enable_rectangle(); });

    on_triggered(m_ui->actionFindSameTool,
                 [this] { m_mouseToolWidget->handle_enable_find_same(); });
  }

  connect(m_mouseToolDock.get(),
          &QDockWidget::visibilityChanged,
          m_ui->actionMouseTools,
          &QAction::setChecked);

  connect(m_tilesetDock.get(),
          &QDockWidget::visibilityChanged,
          m_ui->actionTilesets,
          &QAction::setChecked);

  {
    using CEW = central_editor_widget;
    connect(m_centralWidget, &CEW::request_redraw, this, &W::request_redraw);
    connect(
        m_centralWidget,
            &CEW::request_select_tab, this, &W::request_select_map);
  }

  connect(m_tilesetWidget,
          &tileset_widget::request_new_tileset,
          this,
          &W::request_new_tileset);
}

void window::init_layout() noexcept
{
  const auto loadPrevious = settings_bool("load-previous-layout-on-startup");
  if (loadPrevious && loadPrevious.value()) {
    const auto state = settings_byte_array("last-open-layout-state");
    if (state) {
      restoreState(*state);
    }

    const auto geometry = settings_byte_array("last-open-layout-geometry");
    if (geometry) {
      restoreGeometry(*geometry);
    }
  }
}

void window::reset_dock_layout() noexcept
{
  removeDockWidget(m_mouseToolDock.get());
  removeDockWidget(m_tilesetDock.get());

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_mouseToolDock.get());
  m_mouseToolDock->show();

  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock.get());
  m_tilesetDock->show();
}

void window::hide_all_docks() noexcept
{
  m_mouseToolDock->close();
  m_tilesetDock->close();
}

void window::show_all_docks() noexcept
{
  m_mouseToolDock->show();
  m_tilesetDock->show();
}

auto window::in_editor_mode() const noexcept -> bool
{
  return m_centralWidget->in_editor_mode();
}

}  // namespace tactile::ui
