#include "save_as_dialog.hpp"

#include "preferences.hpp"
#include "standard_paths.hpp"

namespace tactile {

SaveAsDialog::SaveAsDialog(const QString& fileName, QWidget* parent)
    : QFileDialog{parent}
{
  setObjectName(QStringLiteral(u"SaveAsDialog"));
  setAcceptMode(QFileDialog::AcceptSave);

  if (const auto format = prefs::DefaultFormat();
      format.Value() == QStringLiteral(u"JSON"))
  {
    setNameFilter(tr("JSON files (*.json);;TMX files (*.tmx)"));
  }
  else
  {
    setNameFilter(tr("TMX files (*.tmx);;JSON files (*.json)"));
  }

  setWindowTitle(tr("Save As..."));
  setDirectory(GetDocumentsPath());
  selectFile(fileName);
}

}  // namespace tactile
