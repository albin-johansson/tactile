// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/tactile_yml_format.hpp"

#include "tactile/foundation/debug/generic_error.hpp"

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

auto TactileYmlFormat::save_map(const FilePath&,
                                const ir::Map&,
                                const SaveFormatWriteOptions&) const -> Result<void>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TactileYmlFormat::save_tileset(const FilePath&,
                                    const ir::Tileset&,
                                    const SaveFormatWriteOptions&) const -> Result<void>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TactileYmlFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".yml") ||
         extension == TACTILE_NATIVE_STR(".yaml");
}

}  // namespace tactile
