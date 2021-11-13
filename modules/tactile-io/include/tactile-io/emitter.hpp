#pragma once

#include "api.hpp"
#include "ir.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \brief Provides various options for use with the different emitters.
/// \note Some options only have an effect for a subset of the emitters.
enum EmitterOptions : uint32
{
  EmitterOptions_EmbedTilesets = 1u << 0u,
  EmitterOptions_FoldTileData = 1u << 1u,
  EmitterOptions_IndentOutput = 1u << 2u
};

TACTILE_IO_API
void EmitJsonMap(const Map& map, uint32 options);

TACTILE_IO_API
void EmitXmlMap(const Map& map, uint32 options);

TACTILE_IO_API
void EmitYamlMap(const Map& map, uint32 options);

}  // namespace Tactile::IO