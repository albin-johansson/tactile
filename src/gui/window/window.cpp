#include "window.hpp"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QSpacerItem>

#include "about_dialog.hpp"
#include "central_editor_widget.hpp"
#include "create_dock_widget.hpp"
#include "mouse_tool_widget.hpp"
#include "setting.hpp"
#include "setting_identifiers.hpp"
#include "settings_dialog.hpp"
#include "tileset_widget.hpp"
#include "ui_window.h"
#include "widget_size_policy.hpp"

namespace tactile::gui {

window::window(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

  // TODO add mini-map widget

  m_centralWidget = new central_editor_widget{};

  // These are claimed by the dock widgets
  m_mouseToolWidget = new mouse_tool_widget{};
  m_tilesetWidget = new tileset_widget{};

  setCentralWidget(m_centralWidget);  // claims ownership of central widget

  m_mouseToolDock = create_dock_widget(m_mouseToolWidget, "mouseToolDock");
  m_tilesetDock = create_dock_widget(m_tilesetWidget, "tilesetDock");

  m_mouseToolDock->setParent(this);
  m_tilesetDock->setParent(this);

  m_tilesetDock->setWindowTitle("Tilesets");

  set_size_policy(
      m_mouseToolDock, QSizePolicy::Minimum, QSizePolicy::Expanding);
  set_size_policy(m_tilesetDock, QSizePolicy::Expanding);

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_mouseToolDock);
  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock);

  init_connections();
  init_layout();
  enable_startup_view();  // TODO option to reopen last tilemap
}

window::~window() noexcept
{
  delete m_ui;
}

void window::handle_undo_state_update(bool canUndo)
{
  m_ui->actionUndo->setEnabled(canUndo);
}

void window::handle_redo_state_update(bool canRedo)
{
  m_ui->actionRedo->setEnabled(canRedo);
}

void window::handle_undo_text_update(const QString& text)
{
  m_ui->actionUndo->setText("Undo " + text);
}

void window::handle_redo_text_update(const QString& text)
{
  m_ui->actionRedo->setText("Redo " + text);
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

void window::handle_new_map(model::core_model* core, int id)
{
  m_centralWidget->add_new_map_tab(core, "map", id);
  m_centralWidget->select_tab(id);
  if (!in_editor_mode()) {
    enable_editor_view();
    show_all_docks();  // TODO just reopen docks that were visible
    emit request_center_camera();
  }
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);

  settings::set(cfg::window::last_layout_geometry(), saveGeometry());
  settings::set(cfg::window::last_layout_state(), saveState());
}

void window::init_connections() noexcept
{
  connect(m_centralWidget,
          &central_editor_widget::request_remove_tab,
          this,
          [this](int id) noexcept {
            emit request_close_map(id);

            // The tab isn't actually removed yet, this checks if there will be
            // no open tabs
            if (m_centralWidget->open_tabs() == 1) {
              enable_startup_view();
            }
          });

  on_triggered(m_ui->actionExit, [] { QApplication::exit(); });

  on_triggered(m_ui->actionNewMap, &window::request_new_map);

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

  on_triggered(m_ui->actionAboutTactile, &window::handle_display_about_dialog);
  on_triggered(m_ui->actionSettings, &window::handle_display_settings_dialog);

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

  on_triggered(m_ui->actionAddTileset, &window::request_new_tileset);

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
      if (setting<bool> grid{cfg::graphics::grid()}; grid) {
        const auto prev = *grid;
        settings::set(cfg::graphics::grid(), !prev);
        handle_draw();
      }
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
    m_mouseToolGroup = new QActionGroup{this};
    m_mouseToolGroup->setExclusive(true);
    m_mouseToolGroup->addAction(m_ui->actionStampTool);
    m_mouseToolGroup->addAction(m_ui->actionBucketTool);
    m_mouseToolGroup->addAction(m_ui->actionFindSameTool);
    m_mouseToolGroup->addAction(m_ui->actionEraserTool);
    m_mouseToolGroup->addAction(m_ui->actionRectangleTool);

    using MTW = mouse_tool_widget;

    connect(m_mouseToolWidget, &MTW::request_enable_stamp, this, [this] {
      m_ui->actionStampTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::request_enable_bucket, this, [this] {
      m_ui->actionBucketTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::request_enable_eraser, this, [this] {
      m_ui->actionEraserTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::request_enable_rectangle, this, [this] {
      m_ui->actionRectangleTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MTW::request_enable_find_same, this, [this] {
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

  connect(m_mouseToolDock,
          &QDockWidget::visibilityChanged,
          m_ui->actionMouseTools,
          &QAction::setChecked);

  connect(m_tilesetDock,
          &QDockWidget::visibilityChanged,
          m_ui->actionTilesets,
          &QAction::setChecked);

  connect(m_centralWidget,
          &central_editor_widget::request_redraw,
          this,
          &window::request_redraw);

  connect(m_centralWidget,
          &central_editor_widget::request_select_tab,
          this,
          &window::request_select_map);

  connect(m_tilesetWidget,
          &tileset_widget::request_new_tileset,
          this,
          &window::request_new_tileset);
}

void window::init_layout() noexcept
{
  if (setting<QByteArray> geometry{cfg::window::last_layout_geometry()};
      geometry) {
    restoreGeometry(*geometry);
  }

  if (setting<QByteArray> state{cfg::window::last_layout_state()}; state) {
    restoreState(*state);
  }
}

void window::reset_dock_layout() noexcept
{
  removeDockWidget(m_mouseToolDock);
  removeDockWidget(m_tilesetDock);

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_mouseToolDock);
  m_mouseToolDock->show();

  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock);
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

}  // namespace tactile::gui
