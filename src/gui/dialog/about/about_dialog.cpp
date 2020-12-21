#include "about_dialog.hpp"

#include "ui_about_dialog.h"

namespace tactile::gui {

about_dialog::about_dialog(QWidget* parent)
    : QDialog{parent}
    , m_ui{new Ui::about_dialog{}}
{
  m_ui->setupUi(this);
}

about_dialog::~about_dialog() noexcept
{
  delete m_ui;
}

void about_dialog::spawn(QWidget* parent)
{
  about_dialog dialog{parent};
  dialog.exec();
}

}  // namespace tactile::gui
