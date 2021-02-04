#include "import_theme_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile::gui {

import_theme_dialog::import_theme_dialog(QWidget* parent) : QFileDialog{parent}
{
  setWindowTitle(tr("Import theme..."));
  setNameFilter(tr("Tactile theme file (*.json)"));
  setAcceptMode(AcceptOpen);
  setDirectory(get_documents_location());
}

}  // namespace tactile::gui
