#include "select_file_dialog.hpp"

#include <QStandardPaths>

namespace tactile::gui {

select_file_dialog::select_file_dialog(QWidget* parent)
    : QFileDialog{parent}
{
  setFileMode(ExistingFile);
  setViewMode(Detail);
  setWindowTitle(tr("Select file"));
  setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

}  // namespace tactile::gui
