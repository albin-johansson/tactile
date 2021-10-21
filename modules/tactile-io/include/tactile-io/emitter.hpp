#pragma once

#include "api.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

struct EmitterOptions final
{
  bool embed_tilesets{false};
  bool human_readable_output{true};
};

TACTILE_IO_API void EmitJsonMap(const MapData& data, const EmitterOptions& options);

TACTILE_IO_API void EmitXmlMap(const MapData& data);

TACTILE_IO_API void EmitYamlMap(const MapData& data);

}  // namespace Tactile::IO