#pragma once

#include <filesystem>  // path

#include "tactile-io/parse_error.hpp"
#include "tactile-io/parse_ir.hpp"

namespace Tactile::IO {

/**
 * \brief Represents the core parser engine for map files.
 *
 * \details The parser supports the JSON and TMX (XML) Tiled formats.
 *
 * \ingroup io
 *
 * \see `CreateDocumentFromIR()`
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
   * \see `CreateDocumentFromIR()`
   */
  [[nodiscard]] auto GetData() const noexcept -> const MapData&
  {
    return mData;
  }

 private:
  ParseError mError{ParseError::None};
  MapData mData;
};

}  // namespace Tactile::IO
