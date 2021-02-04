#include "export_theme_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile::gui {

export_theme_dialog::export_theme_dialog(QWidget* parent) : QFileDialog{parent}
{
  setWindowTitle(tr("Export theme as..."));
  setNameFilter(tr("Tactile theme file (*.json)"));
  setAcceptMode(AcceptSave);
  setDirectory(get_documents_location());
}

}  // namespace tactile::gui
