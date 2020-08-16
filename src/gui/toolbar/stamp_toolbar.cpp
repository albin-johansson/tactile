#include "stamp_toolbar.hpp"

#include "ui_toolbar_stamp.h"

namespace tactile::ui {

stamp_toolbar::stamp_toolbar(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::StampToolbarUI{}}
{}

stamp_toolbar::~stamp_toolbar() noexcept
{
  delete m_ui;
}

}  // namespace tactile::gui
