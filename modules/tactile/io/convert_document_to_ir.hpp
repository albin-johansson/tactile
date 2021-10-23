#pragma once

#include <tactile-io/parse_ir.hpp>

namespace Tactile {

struct Document;

[[nodiscard]] auto ConvertDocumentToIR(const Document& document) -> IO::MapPtr;

}  // namespace Tactile