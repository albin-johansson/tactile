#pragma once

#include <filesystem>  // path, absolute

#include "aliases/unique.hpp"
#include "map_file.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

/**
 * \brief Represents the core parser engine for map files.
 *
 * \details The parser supports the JSON and TMX (XML) Tiled formats.
 *
 * \ingroup io
 *
 * \see `ToMapDocument()`
 * \see `GetCause(ParseError)`
 */
class MapParser final
{
 public:
  /**
   * \brief Parsers the map file at the specified path.
   *
   * \details The map file should use either of the Tiled JSON or TMX formats.
   *
   * \param path the file path of the map file.
   */
  explicit MapParser(const std::filesystem::path& path);

  /**
   * \brief Indicates whether or not the map was successfully parsed.
   *
   * \return `true` if the map was parsed successfully parsed; `false` otherwise.
   */
  [[nodiscard]] explicit operator bool() const noexcept
  {
    return mError == ParseError::None;
  }

  /**
   * \brief Returns the error code associated with the parser.
   *
   * \return the error code.
   */
  [[nodiscard]] auto GetError() const noexcept -> ParseError
  {
    return mError;
  }

  /**
   * \brief Returns the intermediate representation of the parsed map.
   *
   * \return the intermediate representation of the parsed map.
   *
   * \see `ToMapDocument()`
   */
  [[nodiscard]] auto GetData() const noexcept -> const MapData&
  {
    return mData;
  }

 private:
  ParseError mError{ParseError::None};
  MapData mData;

  [[nodiscard]] auto OpenFile(const std::filesystem::path& path) -> Unique<IMapFile>;

  [[nodiscard]] auto ParseOrientation(const IMapObject& map) -> bool;

  [[nodiscard]] auto ParseInfinite(const IMapObject& map) -> bool;

  [[nodiscard]] auto ParseNextLayerId(const IMapObject& map) -> bool;

  [[nodiscard]] auto ParseNextObjectId(const IMapObject& map) -> bool;

  [[nodiscard]] auto ParseTileWidth(const IMapObject& map) -> bool;

  [[nodiscard]] auto ParseTileHeight(const IMapObject& map) -> bool;
};

}  // namespace Tactile::IO
