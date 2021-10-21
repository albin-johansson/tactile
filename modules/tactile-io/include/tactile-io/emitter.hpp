#pragma once

#include "api.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

/// \brief Provides various options for use with the different emitters.
/// \note Some options only have an effect for a subset of the emitters.
struct EmitterOptions final
{
  bool embed_tilesets{false};        ///< JSON and XML only.
  bool human_readable_output{true};  ///< JSON only
};

TACTILE_IO_API void EmitJsonMap(const MapData& data, const EmitterOptions& options);

TACTILE_IO_API void EmitXmlMap(const MapData& data, const EmitterOptions& options);

TACTILE_IO_API void EmitYamlMap(const MapData& data);

}  // namespace Tactile::IO