#include "import_theme_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile {

ImportThemeDialog::ImportThemeDialog(QWidget* parent) : QFileDialog{parent}
{
  setWindowTitle(tr("Import theme..."));
  setNameFilter(tr("Tactile theme file (*.json)"));
  setAcceptMode(AcceptOpen);
  setDirectory(GetDocumentsPath());
}

}  // namespace tactile
