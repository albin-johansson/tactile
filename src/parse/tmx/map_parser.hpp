#pragma once

#include <QDir>
#include <QFileInfo>
#include <concepts>  // same_as
#include <utility>   // exchange

#include "element_id.hpp"
#include "map_document.hpp"
#include "map_file_type.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::tmx {

// clang-format off

template <typename T>
concept is_object = requires(T t, element_id id, const QString& str)
{
  { t.contains(id)      } -> std::same_as<bool>;
  { t.integer(id)       } -> std::same_as<maybe<int>>;
  { t.integer(id, 1)    } -> std::same_as<maybe<int>>;
  { t.floating(id, 1.0) } -> std::same_as<double>;
  { t.string(id)        } -> std::same_as<maybe<QString>>;
  { t.string(id, str)   } -> std::same_as<QString>;
};

template <typename Parser, typename Document, typename Object>
concept is_parser = is_object<Object> &&
                    requires(Parser parser,
                             const Document& document,
                             const Object& object,
                             core::tile_layer& layer,
                             parse_error& error,
                             const QFileInfo& path)
{
  { parser.root(document) } -> std::same_as<Object>;
  { parser.from_file(path) } -> std::same_as<maybe<Document>>;
  { parser.add_tiles(layer, object, error) } -> std::same_as<bool>;
  { parser.each_tileset(object, [](const Object&) {}) };
  { parser.each_layer(object, [](const Object&) {}) };
};

// clang-format on

template <typename Parser>
class map_parser final
{  // TODO avoid if constexpr blocks for handling the different file types

 public:
  using parser_type = Parser;
  using document_type = typename parser_type::document_type;
  using object_type = typename parser_type::object_type;
  inline constexpr static map_file_type type = Parser::fileType;

  static_assert(is_parser<parser_type, document_type, object_type>);

  explicit map_parser(const QFileInfo& path)
  {
    if (const auto file = open_file(path); file) {
      m_document = new core::map_document{};
      m_document->set_path(path);

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
    // The pointer will be null if the client code claims the document
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
    const auto id = root.integer(element_id::next_layer_id);
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
      if (!root.contains(element_id::tilesets)) {
        return set_error(parse_error::map_missing_tilesets);
      }
    }

    bool ok{true};
    m_parser.each_tileset(root, [&](const object_type& elem) {
      if (ok) {
        const auto firstGid = parse_tileset_first_gid(elem);
        ok = firstGid.has_value();

        if (ok) {
          ok = elem.contains(element_id::source)
                   ? parse_external_tileset(elem, path, *firstGid)
                   : parse_tileset_common(elem, path, *firstGid);
        }
      }

      return ok;
    });

    return ok;
  }

  [[nodiscard]] auto parse_tileset_first_gid(const object_type& object)
      -> maybe<tile_id>
  {
    const auto firstGid = object.integer(element_id::first_gid);
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
    const auto source = object.string(element_id::source);
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
    const auto tw = object.integer(element_id::tile_width);
    if (!tw) {
      return set_error(parse_error::tileset_missing_tile_width);
    }

    const auto th = object.integer(element_id::tile_height);
    if (!th) {
      return set_error(parse_error::tileset_missing_tile_height);
    }

    maybe<QString> relativePath;
    if constexpr (type == map_file_type::tmx) {
      const auto imageElem =
          object->firstChildElement(TACTILE_QSTRING(u"image"));
      relativePath = imageElem.attribute(TACTILE_QSTRING(u"source"));
      if (relativePath->isNull()) {
        return set_error(parse_error::tileset_missing_image_path);
      }
    } else {
      relativePath = object.string(element_id::image);
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

      const auto name = object.string(element_id::name);
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

    if constexpr (type == map_file_type::json) {
      if (!root->contains(u"layers")) {
        return set_error(parse_error::map_missing_layers);
      }
    }

    m_parser.each_layer(root, [&, this](const object_type& elem) {
      if (ok) {
        if constexpr (type == map_file_type::json) {
          if (!elem->contains(u"type")) {
            ok = set_error(parse_error::layer_missing_type);
            return;
          }

          if (elem->value(u"type").toString() != QStringView{u"tilelayer"}) {
            qWarning("Skipping layer in file because it was not a tile layer!");
            return;
          }
        }

        const auto id = elem.integer(element_id::id);
        if (!id) {
          ok = set_error(parse_error::layer_missing_id);
          return;
        }

        const auto rows = elem.integer(element_id::height);
        if (!rows) {
          ok = set_error(parse_error::layer_missing_height);
          return;
        }

        const auto cols = elem.integer(element_id::width);
        if (!cols) {
          ok = set_error(parse_error::layer_missing_width);
          return;
        }

        auto layer = std::make_shared<core::tile_layer>(row_t{*rows}, col_t{*cols});

        layer->set_visible(elem.integer(element_id::visible, 1) == 1);
        layer->set_opacity(elem.floating(element_id::opacity, 1.0));
        layer->set_name(
            elem.string(element_id::name, TACTILE_QSTRING(u"Layer")));

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
