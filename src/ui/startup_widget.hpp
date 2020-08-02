#pragma once
#include <QWidget>

#include "basic_widget_macro.hpp"

namespace Ui {

class StartupWidgetUI;

}

namespace tactile {

TACTILE_BASIC_WIDGET_DECL(StartupWidget, QWidget, Ui::StartupWidgetUI)

}  // namespace tactile
