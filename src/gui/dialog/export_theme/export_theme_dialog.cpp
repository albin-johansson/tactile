#include "export_theme_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile {

ExportThemeDialog::ExportThemeDialog(QWidget* parent) : QFileDialog{parent}
{
  setWindowTitle(tr("Export theme as..."));
  setNameFilter(tr("Tactile theme file (*.json)"));
  setAcceptMode(AcceptSave);
  setDirectory(get_documents_location());
}

}  // namespace tactile
