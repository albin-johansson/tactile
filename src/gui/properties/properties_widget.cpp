#include "properties_widget.hpp"

#include "ui_properties_widget.h"

namespace tactile::gui {

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::properties_widget{}}
{
  m_ui->setupUi(this);
}

}  // namespace tactile::gui
