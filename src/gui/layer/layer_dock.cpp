#include "layer_dock.hpp"

#include "layer_widget.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

layer_dock::layer_dock(QWidget* parent)
    : dock_widget{parent}
    , m_widget{new layer_widget{this}}
{
  setObjectName(TACTILE_QSTRING(u"layer_dock"));
  setWindowTitle(tr("Layers"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setWidget(m_widget);

  // clang-format off
  connect(m_widget, &layer_widget::ui_add_layer, this, &layer_dock::ui_add_layer);
  connect(m_widget, &layer_widget::ui_remove_layer, this, &layer_dock::ui_remove_layer);
  connect(m_widget, &layer_widget::ui_select_layer, this, &layer_dock::ui_select_layer);
  connect(m_widget, &layer_widget::ui_set_layer_visibility, this, &layer_dock::ui_set_layer_visibility);
  connect(m_widget, &layer_widget::ui_set_layer_opacity, this, &layer_dock::ui_set_layer_opacity);
  connect(m_widget, &layer_widget::ui_set_layer_name, this, &layer_dock::ui_set_layer_name);
  connect(m_widget, &layer_widget::ui_move_layer_down, this, &layer_dock::ui_move_layer_down);
  connect(m_widget, &layer_widget::ui_move_layer_up, this, &layer_dock::ui_move_layer_up);
  connect(m_widget, &layer_widget::ui_duplicate_layer, this, &layer_dock::ui_duplicate_layer);
  // clang-format on
}

void layer_dock::added_layer(const layer_id id, const core::layer& layer)
{
  m_widget->added_layer(id, layer);
}

void layer_dock::added_duplicated_layer(const layer_id id,
                                        const core::layer& layer)
{
  m_widget->added_duplicated_layer(id, layer);
}

void layer_dock::removed_layer(const layer_id id)
{
  m_widget->removed_layer(id);
}

void layer_dock::selected_layer(const layer_id id, const core::layer& layer)
{
  m_widget->selected_layer(id, layer);
}

void layer_dock::selected_map(const map_id id,
                              const core::map_document& document)
{
  m_widget->selected_map(id, document);
}

void layer_dock::moved_layer_back(const layer_id id)
{
  m_widget->moved_layer_back(id);
}

void layer_dock::moved_layer_forward(const layer_id id)
{
  m_widget->moved_layer_forward(id);
}

}  // namespace tactile::gui
