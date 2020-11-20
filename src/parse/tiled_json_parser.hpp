#pragma once

#include <QFileInfo>
#include <QJsonDocument>
#include <QString>
#include <optional>  // optional

#include "map_document.hpp"

namespace tactile {

class tiled_json_parser final
{
 public:
  /**
   * \enum tiled_json_parser::parse_error
   *
   * \brief Provides values for with potential JSON map parsing errors.
   *
   * \since 0.1.0
   */
  enum class parse_error
  {
    none,  ///< No error occurred.

    map_file_not_found,    ///< The main map file was not found.
    could_not_parse_json,  ///< Couldn't parse the contents of the JSON file.

    map_missing_next_layer_id,  ///< Map did not feature "nextlayerid".
    map_missing_tilesets,       ///< Map did not feature "tilesets".
    map_missing_layers,         ///< Map did not feature "layers".

    layer_missing_width,         ///< Layer did not feature "width".
    layer_missing_height,        ///< Layer did not feature "height".
    layer_missing_type,          ///< Layer did not feature "type".
    layer_missing_id,            ///< Layer did not feature "id".
    layer_could_not_parse_tile,  ///< Failed to parse layer tile value.

    tileset_missing_first_gid,       ///< Tileset did not feature "firstgid".
    tileset_missing_tile_width,      ///< Tileset did not feature "tilewidth".
    tileset_missing_tile_height,     ///< Tileset did not feature "tileheight".
    tileset_missing_image_path,      ///< Tileset did not feature "image".
    tileset_missing_name,            ///< Tileset did not feature "name".
    could_not_create_tileset,        ///< Couldn't create `tileset` object.
    could_not_read_external_tileset  ///< Could not read external tileset file.
  };

  /**
   * \brief Parses a Tiled JSON map file.
   *
   * \param path the file path of the map file.
   *
   * \since 0.1.0
   */
  explicit tiled_json_parser(const QFileInfo& path);

  /**
   * \brief Destroys the associated map document, if it hasn't been claimed
   * using `take_document()`.
   *
   * \since 0.1.0
   */
  ~tiled_json_parser() noexcept;

  // clang-format off

  /**
   * \brief Returns a pointer to the associated map document.
   *
   * \details You must claim ownership of the returned pointer, preferably by
   * setting the parent of the map document as soon as possible.
   *
   * \return an owning pointer to the associated map document, can be null.
   *
   * \since 0.1.0
   */
  [[nodiscard("Discarding the pointer is a memory leak!")]]
  auto take_document() -> core::map_document*;

  // clang-format on

  /**
   * \brief Returns the code associated with any potential error when parsing
   * the JSON file.
   *
   * \return the error code associated with the parser.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto error_code() const -> parse_error;

  /**
   * \brief Indicates whether or not the parser holds a valid map document.
   *
   * \return `true` if there were no error when parsing the file; `false`
   * otherwise.
   *
   * \since 0.1.0
   */
  explicit operator bool() const noexcept;

 private:
  core::map_document* m_document{};
  parse_error m_error{parse_error::none};

  [[nodiscard]] auto set_error(const parse_error errorCode) noexcept -> bool
  {
    m_error = errorCode;
    return false;
  }

  [[nodiscard]] auto open_file(const QFileInfo& path)
      -> std::optional<QJsonDocument>;

  [[nodiscard]] auto parse_next_layer_id(const QJsonObject& root) -> bool;

  [[nodiscard]] auto parse_tilesets(const QJsonObject& root,
                                    const QFileInfo& path) -> bool;

  [[nodiscard]] auto parse_tileset_first_gid(const QJsonObject& object)
      -> std::optional<tile_id>;

  [[nodiscard]] auto parse_external_tileset(const QJsonObject& object,
                                            const QFileInfo& path,
                                            tile_id firstGid) -> bool;

  [[nodiscard]] auto parse_tileset_common(const QJsonObject& object,
                                          const QFileInfo& path,
                                          tile_id firstGid) -> bool;

  [[nodiscard]] auto parse_layers(const QJsonObject& root) -> bool;
};

}  // namespace tactile
