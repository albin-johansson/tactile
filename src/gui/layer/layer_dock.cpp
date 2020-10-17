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
  connect(m_widget, &layer_widget::ui_requested_new_layer, this, &layer_dock::ui_requested_new_layer);
  connect(m_widget, &layer_widget::ui_requested_remove_layer, this, &layer_dock::ui_requested_remove_layer);
  connect(m_widget, &layer_widget::ui_selected_layer, this, &layer_dock::ui_selected_layer);
  connect(m_widget, &layer_widget::ui_set_layer_visibility, this, &layer_dock::ui_set_layer_visibility);
  // clang-format on
}

}  // namespace tactile::gui
