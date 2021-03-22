#include "open_map_dialog.hpp"

#include <QStandardPaths>

#include "preferences.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

open_map_dialog::open_map_dialog(QWidget* parent) : QFileDialog{parent}
{
  setAcceptMode(QFileDialog::AcceptOpen);
  setFileMode(QFileDialog::ExistingFiles);

  if (const auto format = prefs::saves::default_format();
      format.value() == TACTILE_QSTRING(u"JSON"))
  {
    setNameFilter(tr("JSON files (*.json);;TMX files (*.tmx)"));
  }
  else
  {
    setNameFilter(tr("TMX files (*.tmx);;JSON files (*.json)"));
  }

  setWindowTitle(tr("Open..."));
  setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

}  // namespace tactile::gui
