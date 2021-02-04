#include "about_dialog.hpp"

#include "init_ui.hpp"
#include "ui_about_dialog.h"

namespace tactile::gui {

about_dialog::about_dialog(QWidget* parent)
    : QDialog{parent}
    , m_ui{init_ui<Ui::about_dialog>(this)}
{}

about_dialog::~about_dialog() noexcept = default;

void about_dialog::spawn(QWidget* parent)
{
  about_dialog dialog{parent};
  dialog.exec();
}

}  // namespace tactile::gui
