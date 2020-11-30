#include "save_as_dialog.hpp"

#include <QStandardPaths>

#include "preferences.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

save_as_dialog::save_as_dialog(const QString& fileName, QWidget* parent)
    : QFileDialog{parent}
{
  setAcceptMode(QFileDialog::AcceptSave);

  if (const auto format = prefs::saves::default_format();
      format.value() == TACTILE_QSTRING(u"JSON")) {
    setNameFilter(tr("JSON files (*.json);;TMX files (*.tmx)"));
  } else {
    setNameFilter(tr("TMX files (*.tmx);;JSON files (*.json)"));
  }

  setWindowTitle(tr("Save As..."));
  setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  selectFile(fileName);
}

}  // namespace tactile::gui
