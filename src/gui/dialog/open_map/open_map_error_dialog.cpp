#include "open_map_error_dialog.hpp"

#include "init_ui.hpp"
#include "ui_open_map_error_dialog.h"

namespace tactile::gui {

open_map_error_dialog::open_map_error_dialog(QWidget* parent)
    : QDialog{parent}
    , m_ui{init_ui<Ui::open_map_error_dialog>(this)}
{}

open_map_error_dialog::~open_map_error_dialog() noexcept = default;

void open_map_error_dialog::set_file(const QFileInfo& path)
{
  m_ui->filePathLabel->setText(path.fileName());
}

void open_map_error_dialog::set_error_message(const QString& message)
{
  m_ui->errorMessageLabel->setText(message);
}

}  // namespace tactile::gui
