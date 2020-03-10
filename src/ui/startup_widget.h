#pragma once
#include <QWidget>

#include "basic_widget_macro.h"

namespace Ui {

class StartupWidgetUI;

}

namespace tactile {

TACTILE_BASIC_WIDGET_DECL(StartupWidget, QWidget, Ui::StartupWidgetUI)

}  // namespace tactile
