#pragma once

#include "io/parsing/parse_error.hpp"

namespace Tactile {

void UpdateMapImportErrorDialog();

void OpenMapImportErrorDialog(IO::ParseError error);

}  // namespace Tactile
