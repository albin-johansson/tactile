#include "layer_dock.hpp"

namespace tactile::gui {

layer_dock::layer_dock(QWidget* parent)
    : QDockWidget{parent},
      m_widget{new layer_widget{this}}
{
  setObjectName(QStringLiteral(u"layer_dock"));
  setWindowTitle(tr("Layers"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setWidget(m_widget);

  // clang-format off
  connect(m_widget, &layer_widget::ui_add_layer, this, &layer_dock::ui_add_layer);
  connect(m_widget, &layer_widget::ui_remove_layer, this, &layer_dock::ui_remove_layer);
  connect(m_widget, &layer_widget::ui_selected_layer, this, &layer_dock::ui_selected_layer);
  connect(m_widget, &layer_widget::ui_set_layer_visibility, this, &layer_dock::ui_set_layer_visibility);
  connect(m_widget, &layer_widget::ui_set_layer_opacity, this, &layer_dock::ui_set_layer_opacity);
  connect(m_widget, &layer_widget::ui_set_layer_name, this, &layer_dock::ui_set_layer_name);
  connect(m_widget, &layer_widget::ui_move_layer_down, this, &layer_dock::ui_move_layer_down);
  connect(m_widget, &layer_widget::ui_move_layer_up, this, &layer_dock::ui_move_layer_up);
  connect(m_widget, &layer_widget::ui_duplicate_layer, this, &layer_dock::ui_duplicate_layer);
  // clang-format on
}

}  // namespace tactile::gui
