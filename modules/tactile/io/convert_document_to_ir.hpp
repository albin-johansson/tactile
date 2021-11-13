#pragma once

#include <tactile_io.hpp>

namespace Tactile {

struct Document;

[[nodiscard]] auto ConvertDocumentToIR(const Document& document) -> IO::MapPtr;

}  // namespace Tactile