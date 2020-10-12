#include "open_map_dialog.hpp"

#include <QStandardPaths>

namespace tactile::gui {

open_map_dialog::open_map_dialog(QWidget* parent) : QFileDialog{parent}
{
  setAcceptMode(QFileDialog::AcceptOpen);
  setFileMode(QFileDialog::ExistingFiles);
  setNameFilter(QStringLiteral(u"TMX files (*.tmx);;JSON files (*.json)"));
  setWindowTitle(QStringLiteral(u"Open..."));
  setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

}  // namespace tactile::gui
