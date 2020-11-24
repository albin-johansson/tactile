#pragma once

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtXml>
#include <concepts>  // same_as
#include <utility>   // exchange

#include "json_element.hpp"
#include "json_utils.hpp"
#include "map_document.hpp"
#include "map_file_type.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tmx_element.hpp"
#include "xml_engine.hpp"
#include "xml_utils.hpp"

namespace tactile::tmx {

// clang-format off

template <typename T>
concept is_object = requires(T t, const QString& str, int i, double d, element_type elem)
{
  { t.contains(str) } -> std::same_as<bool>;
  { t.integer(str) } -> std::same_as<maybe<int>>;
  { t.integer(elem) } -> std::same_as<maybe<int>>;
  { t.integer(str, i) } -> std::same_as<int>;
  { t.floating(str, d) } -> std::same_as<double>;
  { t.string(str) } -> std::same_as<maybe<QString>>;
  { t.string(str, str) } -> std::same_as<QString>;
};

template <typename P, typename D, typename E>
concept is_parser = requires(P parser,
                             const D& document,
                             const E& elem,
                             core::layer& layer,
                             parse_error& error,
                             const QFileInfo& path)
{
  { parser.root(document) } -> std::same_as<E>;
  { parser.from_file(path) } -> std::same_as<maybe<D>>;
  { parser.add_tiles(layer, elem, error) } -> std::same_as<bool>;
  { parser.each_tileset(elem, [](const E&) {}) };
  { parser.each_layer(elem, [](const E&) {}) };
};

// clang-format on

template <typename Parser>
class map_parser final
{
 public:
  using parser_type = Parser;
  using document_type = typename parser_type::document_type;
  using object_type = typename parser_type::object_type;
  inline constexpr static map_file_type type = Parser::fileType;

  explicit map_parser(const QFileInfo& path)
  {
    if (const auto file = open_file(path); file) {
      m_document = new core::map_document{};

      const auto root = m_parser.root(*file);
      if (!parse_next_layer_id(root)) {
        return;
      }

      if (!parse_tilesets(root, path)) {
        return;
      }

      if (!parse_layers(root)) {
        return;
      }
    }
  }

  ~map_parser() noexcept
  {
    delete m_document;
  }

  // clang-format off

  /**
   * \brief Returns a pointer to the associated map document.
   *
   * \details You must claim ownership of the returned pointer, preferably by
   * setting the parent of the map document as soon as possible.
   *
   * \return an owning pointer to the associated map document.
   *
   * \since 0.1.0
   */
  [[nodiscard("Discarding the pointer is a memory leak!")]]
  auto take_document() -> core::map_document*
  {
    return std::exchange(m_document, nullptr);
  }

  // clang-format on

  /**
   * \brief Returns the code associated with any potential error when parsing
   * the file.
   *
   * \return the error code associated with the parser.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto error_code() const noexcept -> parse_error
  {
    return m_error;
  }

  /**
   * \brief Indicates whether or not the parser holds a valid map document.
   *
   * \return `true` if there were no error when parsing the file; `false`
   * otherwise.
   *
   * \since 0.1.0
   */
  explicit operator bool() const noexcept
  {
    return m_error == parse_error::none;
  }

 private:
  core::map_document* m_document{};
  parse_error m_error{parse_error::none};
  parser_type m_parser;

  [[nodiscard]] auto set_error(const parse_error error) noexcept -> bool
  {
    m_error = error;
    return false;
  }

  [[nodiscard]] auto open_file(const QFileInfo& path) -> maybe<document_type>
  {
    if (!path.exists()) {
      m_error = parse_error::map_file_not_found;
      return std::nullopt;
    }

    auto contents = m_parser.from_file(path);

    if (!contents) {
      m_error = parse_error::could_not_parse_file;
    }

    return contents;
  }

  [[nodiscard]] auto parse_next_layer_id(const object_type& root) -> bool
  {
    const auto id = root.integer(element_type::next_layer_id);
    if (id) {
      m_document->set_next_layer_id(layer_id{*id});
      return true;
    } else {
      return set_error(parse_error::map_missing_next_layer_id);
    }
  }

  [[nodiscard]] auto parse_tilesets(const object_type& root,
                                    const QFileInfo& path) -> bool
  {
    if constexpr (type == map_file_type::json) {
      if (!root.contains(u"tilesets")) {
        return set_error(parse_error::map_missing_tilesets);
      }
    }

    bool ok{true};
    m_parser.each_tileset(root, [&](const object_type& elem) {
      if (ok) {
        const auto firstGid = parse_tileset_first_gid(elem);
        if (!firstGid) {
          return false;
        }

        if (elem.contains(QStringLiteral(u"source"))) {
          // external tileset
          if (!parse_external_tileset(elem, path, *firstGid)) {
            return false;
          }
        } else {
          // embedded tileset
          if (!parse_tileset_common(elem, path, *firstGid)) {
            return false;
          }
        }
        return true;
      }
      return false;
    });

    return true;
  }

  [[nodiscard]] auto parse_tileset_first_gid(const object_type& object)
      -> maybe<tile_id>
  {
    const auto firstGid = object.integer(element_type::first_gid);
    if (firstGid) {
      return tile_id{*firstGid};
    } else {
      m_error = parse_error::tileset_missing_first_gid;
      return std::nullopt;
    }
  }

  [[nodiscard]] auto parse_external_tileset(const object_type& object,
                                            const QFileInfo& path,
                                            const tile_id firstGid) -> bool
  {
    const auto source = object.string(QStringLiteral(u"source"));
    const auto external =
        m_parser.from_file(path.dir().absoluteFilePath(*source));
    if (external) {
      return parse_tileset_common(m_parser.root(*external), path, firstGid);
    } else {
      return set_error(parse_error::could_not_read_external_tileset);
    }
  }

  [[nodiscard]] auto parse_tileset_common(const object_type& object,
                                          const QFileInfo& path,
                                          const tile_id firstGid) -> bool
  {
    const auto tw = object.integer(element_type::tile_width);
    if (!tw) {
      return set_error(parse_error::tileset_missing_tile_width);
    }

    const auto th = object.integer(element_type::tile_height);
    if (!th) {
      return set_error(parse_error::tileset_missing_tile_height);
    }

    maybe<QString> relativePath;
    if constexpr (type == map_file_type::tmx) {
      const auto imageElem =
          object->firstChildElement(QStringLiteral(u"image"));
      relativePath = imageElem.attribute(QStringLiteral(u"source"));
      if (relativePath->isNull()) {
        return set_error(parse_error::tileset_missing_image_path);
      }
    } else {
      relativePath = object.string(QStringLiteral(u"image"));
      if (!relativePath) {
        return set_error(parse_error::tileset_missing_image_path);
      }
    }

    const auto absolutePath = path.dir().absoluteFilePath(*relativePath);
    if (!QFileInfo{absolutePath}.exists()) {
      return set_error(parse_error::external_tileset_does_not_exist);
    }

    try {
      auto tileset = std::make_shared<core::tileset>(firstGid,
                                                     absolutePath,
                                                     tile_width{*tw},
                                                     tile_height{*th});

      const auto name = object.string(QStringLiteral(u"name"));
      if (!name) {
        return set_error(parse_error::tileset_missing_name);
      }

      tileset->set_name(*name);
      tileset->set_path(absolutePath);

      m_document->add_tileset(std::move(tileset));
    } catch (...) {
      return set_error(parse_error::could_not_create_tileset);
    }

    return true;
  }

  [[nodiscard]] auto parse_layers(const object_type& root) -> bool
  {
    bool first{true};
    bool ok{true};
    m_parser.each_layer(root, [&, this](const object_type& elem) {
      if (ok) {
        const auto id = elem.integer(element_type::id);
        if (!id) {
          ok = set_error(parse_error::layer_missing_id);
          return;
        }

        const auto rows = elem.integer(element_type::height);
        if (!rows) {
          ok = set_error(parse_error::layer_missing_height);
          return;
        }

        const auto cols = elem.integer(element_type::width);
        if (!cols) {
          ok = set_error(parse_error::layer_missing_width);
          return;
        }

        auto layer = std::make_shared<core::layer>(row_t{*rows}, col_t{*cols});

        layer->set_visible(elem.integer(QStringLiteral(u"visible"), 1) == 1);
        layer->set_opacity(elem.floating(QStringLiteral(u"opacity"), 1.0));
        layer->set_name(
            elem.string(QStringLiteral(u"name"), QStringLiteral(u"Layer")));

        if (!m_parser.add_tiles(*layer, elem, m_error)) {
          ok = false;
          return;
        }

        const layer_id layerId{*id};
        m_document->add_layer(layerId, layer);

        if (first) {
          first = false;
          m_document->select_layer(layerId);
        }
      }
    });

    return true;
  }
};

}  // namespace tactile::tmx
