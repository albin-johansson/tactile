// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/tactile_yml_format.hpp"

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/io/stream.hpp"
#include "tactile/tactile-yml-format/emit/map_emitter.hpp"
#include "tactile/tactile-yml-format/emit/tileset_emitter.hpp"

namespace tactile {

auto TactileYmlFormat::load_map(const FilePath&, const SaveFormatReadOptions&) const
    -> Result<ir::Map>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TactileYmlFormat::load_tileset(const FilePath&, const SaveFormatReadOptions&) const
    -> Result<ir::Tileset>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TactileYmlFormat::save_map(const FilePath& map_path,
                                const ir::Map& map,
                                const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  yml_format::emit_map(emitter, map, options);

  return stream_to_file(emitter.c_str(),
                        map_path,
                        StreamToFileOptions {.indentation = 0, .binary_mode = false});
}

auto TactileYmlFormat::save_tileset(const FilePath& tileset_path,
                                    const ir::Tileset& tileset,
                                    const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  YAML::Emitter emitter;
  emitter.SetIndent(2);

  yml_format::emit_tileset(emitter, tileset, options);

  return stream_to_file(emitter.c_str(),
                        tileset_path,
                        StreamToFileOptions {.indentation = 0, .binary_mode = false});
}

auto TactileYmlFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".yml") ||
         extension == TACTILE_NATIVE_STR(".yaml");
}

}  // namespace tactile
