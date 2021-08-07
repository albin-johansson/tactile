#include "parse_csv.hpp"

#include <sstream>  // istringstream
#include <string>   // string, erase, getline
#include <vector>   // vector

#include "core/map/map_position.hpp"
#include "utils/from_string.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto GetTokens(const czstring csv) -> std::vector<std::string>
{
  std::istringstream stream{csv};
  std::vector<std::string> tokens;

  std::string token;
  while (std::getline(stream, token, ','))
  {
    std::erase(token, '\n');
    tokens.push_back(std::move(token));
    token.clear();
  }

  return tokens;
}

}  // namespace

auto ParseCSV(const czstring csv, const col_t nCols, TileMatrix& matrix)
    -> ParseError
{
  int index{};
  for (const auto& token : GetTokens(csv))
  {
    if (const auto id = FromString<tile_id::value_type>(token))
    {
      const auto pos = MapPosition::FromIndex(index, nCols);
      matrix.at(pos.GetRowIndex()).at(pos.GetColumnIndex()) = tile_id{*id};
      ++index;
    }
    else
    {
      return ParseError::CouldNotParseTiles;
    }
  }

  return ParseError::None;
}

}  // namespace Tactile::IO
