#include "standard_paths.hpp"

#include <QStandardPaths>  // QStandardPaths

namespace tactile {

auto get_documents_location() -> QString
{
  return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

}  // namespace tactile
