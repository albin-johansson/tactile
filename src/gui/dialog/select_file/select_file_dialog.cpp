#include "select_file_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile {

SelectFileDialog::SelectFileDialog(QWidget* parent) : QFileDialog{parent}
{
  setFileMode(ExistingFile);
  setViewMode(Detail);
  setWindowTitle(tr("Select file"));
  setDirectory(GetDocumentsPath());
}

}  // namespace tactile
