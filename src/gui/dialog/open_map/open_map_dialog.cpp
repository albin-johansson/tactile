#include "open_map_dialog.hpp"

#include <QStandardPaths>

#include "preferences.hpp"
#include "standard_paths.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

OpenMapDialog::OpenMapDialog(QWidget* parent) : QFileDialog{parent}
{
  setAcceptMode(QFileDialog::AcceptOpen);
  setFileMode(QFileDialog::ExistingFiles);

  if (const auto format = prefs::DefaultFormat();
      format.value() == TACTILE_QSTRING(u"JSON"))
  {
    setNameFilter(tr("JSON files (*.json);;TMX files (*.tmx)"));
  }
  else
  {
    setNameFilter(tr("TMX files (*.tmx);;JSON files (*.json)"));
  }

  setWindowTitle(tr("Open..."));
  setDirectory(GetDocumentsPath());
}

}  // namespace tactile
