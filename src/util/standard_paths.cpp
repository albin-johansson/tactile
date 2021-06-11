#include "standard_paths.hpp"

#include <QStandardPaths>  // QStandardPaths

namespace tactile {

auto GetDocumentsPath() -> QString
{
  return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

}  // namespace tactile
