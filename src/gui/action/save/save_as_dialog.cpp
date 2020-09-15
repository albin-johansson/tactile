#include "save_as_dialog.hpp"

#include <qstandardpaths.h>

namespace tactile::gui {

save_as_dialog::save_as_dialog(const QString& fileName, QWidget* parent)
    : QFileDialog{parent}
{
  setAcceptMode(QFileDialog::AcceptSave);
  setNameFilter(QStringLiteral(u"TMX files (*.tmx);;JSON files (*.json)"));
  setWindowTitle(QStringLiteral(u"Save As..."));
  setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  selectFile(fileName);
}

}  // namespace tactile::gui
