#pragma once
#include <QWidget>

#include "basic_widget_macro.hpp"

namespace Ui {

class StartupWidgetUI;

}

namespace tactile::ui {

TACTILE_BASIC_WIDGET_DECL(startup_widget, QWidget, Ui::StartupWidgetUI)

}  // namespace tactile::ui
