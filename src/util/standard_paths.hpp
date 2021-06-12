#pragma once

#include <QString>  // QString

namespace tactile {

[[nodiscard]] auto GetDocumentsPath() -> QString;

[[nodiscard]] auto GetPicturesPath() -> QString;

}  // namespace tactile
