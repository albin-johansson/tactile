#include "map_editor_context_menu.hpp"

namespace tactile::gui {

map_editor_context_menu::map_editor_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_showProperties{addAction(tr("Show properties..."))}
{
  // clang-format off
  connect(m_showProperties, &QAction::triggered, this, &map_editor_context_menu::show_properties);
  // clang-format on
}

}  // namespace tactile::gui
