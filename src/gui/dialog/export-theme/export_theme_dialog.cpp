#include "export_theme_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile {

ExportThemeDialog::ExportThemeDialog(QWidget* parent) : QFileDialog{parent}
{
  setObjectName(QStringLiteral(u"ExportThemeDialog"));
  setWindowTitle(tr("Export theme as..."));
  setNameFilter(tr("Tactile theme file (*.json)"));
  setAcceptMode(AcceptSave);
  setDirectory(GetDocumentsPath());
}

}  // namespace tactile
