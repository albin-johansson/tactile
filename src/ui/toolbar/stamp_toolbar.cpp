#include "stamp_toolbar.h"

#include "ui_toolbar_stamp.h"

namespace tactile {

StampToolbar::StampToolbar(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::StampToolbarUI{}}
{

}

StampToolbar::~StampToolbar() noexcept
{
  delete m_ui;
}

}  // namespace tactile
