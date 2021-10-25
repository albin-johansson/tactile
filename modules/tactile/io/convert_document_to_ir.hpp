#pragma once

#include <tactile-io/ir.hpp>

namespace Tactile {

struct Document;

[[nodiscard]] auto ConvertDocumentToIR(const Document& document) -> IO::MapPtr;

}  // namespace Tactile