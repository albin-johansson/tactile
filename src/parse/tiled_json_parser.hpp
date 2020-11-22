#pragma once

#include <QFileInfo>
#include <QJsonDocument>
#include <QString>
#include <optional>  // optional

#include "map_document.hpp"
#include "parse_error.hpp"

namespace tactile {

class tiled_json_parser final
{
 public:
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
