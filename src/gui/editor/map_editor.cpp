#include "map_editor.hpp"

#include "map_tab_widget.hpp"
#include "startup_widget.hpp"
#include "ui_map_editor.h"

namespace tactile::gui {

map_editor::map_editor(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::map_editor{}}
{
  m_ui->setupUi(this);

  m_tabWidget = new map_tab_widget{this};

  m_startupID = m_ui->stackedWidget->addWidget(new startup_widget{this});
  m_editorID = m_ui->stackedWidget->addWidget(m_tabWidget);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  init_connections();
}

map_editor::~map_editor() noexcept
{
  delete m_ui;
}

void map_editor::init_connections()
{
  // clang-format off
  connect(this, &map_editor::theme_changed, m_tabWidget, &map_tab_widget::theme_changed);
  connect(m_tabWidget, &map_tab_widget::currentChanged, this, &map_editor::tab_changed);
  connect(m_tabWidget, &map_tab_widget::ui_remove_map, this, &map_editor::ui_remove_map);
  connect(m_tabWidget, &map_tab_widget::increase_zoom, this, &map_editor::increase_zoom);
  connect(m_tabWidget, &map_tab_widget::decrease_zoom, this, &map_editor::decrease_zoom);
  connect(m_tabWidget, &map_tab_widget::mouse_pressed, this, &map_editor::mouse_pressed);
  connect(m_tabWidget, &map_tab_widget::mouse_moved, this, &map_editor::mouse_moved);
  connect(m_tabWidget, &map_tab_widget::mouse_released, this, &map_editor::mouse_released);
  connect(m_tabWidget, &map_tab_widget::mouse_entered, this, &map_editor::mouse_entered);
  connect(m_tabWidget, &map_tab_widget::mouse_exited, this, &map_editor::mouse_exited);
  // clang-format on
}

void map_editor::enable_stamp_preview(const core::position& position)
{
  m_tabWidget->enable_stamp_preview(position);
}

void map_editor::disable_stamp_preview()
{
  m_tabWidget->disable_stamp_preview();
}

void map_editor::add_map_tab(core::map_document* map,
                             map_id id,
                             const QString& title)
{
  m_tabWidget->add_map_tab(map, id, title);
}

void map_editor::select_tab(map_id id)
{
  m_tabWidget->select_tab(id);
}

void map_editor::close_tab(map_id id) noexcept
{
  m_tabWidget->remove_map_tab(id);
}

void map_editor::center_viewport()
{
  m_tabWidget->center_map();
}

void map_editor::move_map(int dx, int dy) noexcept
{
  m_tabWidget->move_map(dx, dy);
}

void map_editor::force_redraw()
{
  m_tabWidget->force_redraw();
}

void map_editor::enable_startup_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_startupID);
}

void map_editor::enable_editor_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_editorID);
}

auto map_editor::in_editor_mode() const -> bool
{
  return m_ui->stackedWidget->currentIndex() == m_editorID;
}

auto map_editor::active_tab_id() const -> std::optional<map_id>
{
  return m_tabWidget->active_tab_id();
}

auto map_editor::active_tab_name() const -> std::optional<QString>
{
  return m_tabWidget->active_tab_name();
}

auto map_editor::tab_count() const -> int
{
  return m_tabWidget->count();
}

void map_editor::tab_changed(int index)
{
  if (const auto id = m_tabWidget->id_from_index(index); id) {
    emit ui_select_map(*id);
  }
}

}  // namespace tactile::gui
