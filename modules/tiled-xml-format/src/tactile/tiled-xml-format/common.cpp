// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/common.hpp"

#include <ostream>  // ostream

#include "tactile/foundation/io/stream.hpp"

namespace pugi {

auto operator<<(std::ostream& stream, const xml_document& document) -> std::ostream&
{
  document.save(stream, " ");
  return stream;
}

}  // namespace pugi

namespace tactile::tiled::tmx {

// TODO XmlError?

auto load_xml_file(const FilePath& path) -> Result<pugi::xml_document>
{
  pugi::xml_document document {};
  const auto parse_result =
      document.load_file(path.c_str(), pugi::parse_default | pugi::parse_trim_pcdata);

  if (parse_result.status == pugi::status_ok) {
    return document;
  }

  return unexpected(make_generic_error(GenericError::kIOError));
}

auto save_xml_file(const pugi::xml_document& document,
                   const FilePath& path,
                   const int indentation) -> Result<void>
{
  const StreamToFileOptions stream_options {
    .indentation = indentation,
    .binary_mode = false,
  };

  return stream_to_file(document, path, stream_options);
}

}  // namespace tactile::tiled::tmx
