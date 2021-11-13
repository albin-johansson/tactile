#include "emit.hpp"

#include "io_utils.hpp"
#include "ir_definitions.hpp"
#include "ir_map.hpp"

namespace Tactile::IO {

auto GetDefaultEmitterOptions() -> EmitterOptions
{
  return EmitterOption_IndentOutput;
}

auto EmitMap(const Map& map, const EmitterOptions options) -> bool
{
  const auto* path = GetPath(map);
  const auto format = DeduceFormat(path);

  switch (format) {
    case MapFormat::Yaml:
      EmitYamlMap(map, options);
      return true;

    case MapFormat::Json:
      EmitJsonMap(map, options);
      return true;

    case MapFormat::Xml:
      EmitXmlMap(map, options);
      return true;

    default:
      return false;
  }
}

}  // namespace Tactile::IO
