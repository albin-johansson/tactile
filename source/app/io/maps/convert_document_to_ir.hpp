#pragma once

#include "ir.hpp"

namespace tactile {

struct Document;

[[nodiscard]] auto convert_document_to_ir(const Document& document) -> ir::map_data;

}  // namespace tactile