#ifndef TACTILE_IO_EMIT_HPP_
#define TACTILE_IO_EMIT_HPP_

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/**
 * \brief Provides various options for use with the different emitters.
 *
 * \note Some options only have an effect for a subset of the emitters.
 *
 * \details `EmitterOption_EmbedTilesets` (*JSON* and *XML* only) ensures that tileset
 * data is stored in the main map file. This is not recommended since it makes tilesets
 * hard to reuse. On the other hand, this ensures that all you need is stored in one file.
 *
 * \details `EmitterOption_FoldTileData` (*YAML* and *XML* only) will force tile layers to
 * arrange their tile data row-by-row, instead of storing the tile identifiers as a single
 * stream of identifiers. Folding the tile data makes the tile data much easier to read by
 * humans. However, this doesn't scale very well for larger maps, in which enabling this
 * feature might actually decrease the overall readability of the map file. It's therefore
 * recommended to not fold the tile data. And you shouldn't have to care about the
 * legibility of the raw tile data in order to be able to edit it by hand, that's what the
 * Tactile editor is for!
 *
 * \details `EmitterOption_IndentOutput` (*JSON* and *XML* only) simply let's you decide
 * whether or not the output is indented. Not indenting the output will obviously save
 * space, but it will make the save files completely unreadable by humans. It's probably
 * wise to keep this option enabled.
 */
enum EmitterOption : uint32
{
  EmitterOption_EmbedTilesets = 1u << 0u,  ///< Store tilesets in the main map file.
  EmitterOption_FoldTileData = 1u << 1u,   ///< Make tile layer data to be more readable.
  EmitterOption_IndentOutput = 1u << 2u    ///< Make files human-readable.
};

using EmitterOptions = uint32;

/**
 * \brief Returns the recommended emitter options.
 *
 * \return a mask of the recommended options.
 *
 * \see `EmitterOption`
 */
TACTILE_IO_API_QUERY
auto GetDefaultEmitterOptions() -> EmitterOptions;

TACTILE_IO_API
void EmitJsonMap(const Map& map, EmitterOptions options = GetDefaultEmitterOptions());

TACTILE_IO_API
void EmitXmlMap(const Map& map, EmitterOptions options = GetDefaultEmitterOptions());

TACTILE_IO_API
void EmitYamlMap(const Map& map, EmitterOptions options = GetDefaultEmitterOptions());

/**
 * \brief Attempts to save a map, using the associated destination path information.
 *
 * \details This function has no effect if the supplied map doesn't provide a valid
 * destination path. Make sure to use `SetPath(Map&)` to configure the destination path.
 *
 * \details The map format to use is deduced from the extension of the associated file
 * path. Supported file extensions are `.yml`, `.yaml`, `.json`, `.xml`, and `.tmx`.
 *
 * \param map the map that will be saved.
 * \param options the emitter options that will be used.
 *
 * \return `true` if an _attempt_ was made to save the map data; `false` otherwise.
 *
 * \see `EmitterOption`
 * \see `EmitYamlMap()`
 * \see `EmitJsonMap()`
 * \see `EmitXmlMap()`
 */
TACTILE_IO_API
auto EmitMap(const Map& map, EmitterOptions options = GetDefaultEmitterOptions()) -> bool;

}  // namespace Tactile::IO

#endif  // TACTILE_IO_EMIT_HPP_
