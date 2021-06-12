#include "standard_paths.hpp"

#include <QStandardPaths>  // QStandardPaths

namespace tactile {

auto GetDocumentsPath() -> QString
{
  return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

auto GetPicturesPath() -> QString
{
  return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
}

}  // namespace tactile
