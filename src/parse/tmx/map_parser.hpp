#pragma once

#include <QDir>
#include <QFileInfo>
#include <concepts>  // same_as
#include <ranges>    // all_of
#include <utility>   // exchange
#include <vector>    // vector

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
  { t.floating(id)      } -> std::same_as<maybe<double>>;
  { t.floating(id, 1.0) } -> std::same_as<double>;
  { t.string(id)        } -> std::same_as<maybe<QString>>;
  { t.string(id, str)   } -> std::same_as<QString>;
  { t.boolean(id)       } -> std::same_as<maybe<bool>>;
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
  { parser.root(document)                      } -> std::same_as<Object>;
  { parser.layers(object)                      } -> std::same_as<std::vector<Object>>;
  { parser.tilesets(object)                    } -> std::same_as<std::vector<Object>>;
  { parser.properties(object)                  } -> std::same_as<std::vector<Object>>;
  { parser.from_file(path)                     } -> std::same_as<maybe<Document>>;
  { parser.add_tiles(layer, object, error)     } -> std::same_as<bool>;
  { parser.contains_layers(object)             } -> std::same_as<bool>;
  { parser.contains_tilesets(object)           } -> std::same_as<bool>;
  { parser.validate_layer_type(object)         } -> std::same_as<bool>;
  { parser.tileset_image_relative_path(object) } -> std::same_as<maybe<QString>>;
  { parser.assume_string_property(object)      } -> std::same_as<bool>;
  { parser.is_tile_layer(object)               } -> std::same_as<bool>;
  { parser.is_object_layer(object)             } -> std::same_as<bool>;
};

// clang-format on

template <typename Parser>
class map_parser final
{
 public:
  using parser_type = Parser;
  using document_type = typename parser_type::document_type;
  using object_type = typename parser_type::object_type;
  inline constexpr static map_file_type type = parser_type::fileType;

  static_assert(is_parser<parser_type, document_type, object_type>,
                "The supplied type isn't a parser engine!");

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

      if (!parse_map_properties(root)) {
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

  [[nodiscard]] auto with_error(const parse_error error) noexcept -> bool
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
      return with_error(parse_error::map_missing_next_layer_id);
    }
  }

  [[nodiscard]] auto parse_tileset(const object_type& object,
                                   const QFileInfo& path) -> bool
  {
    if (const auto first = parse_tileset_first_gid(object)) {
      return object.contains(element_id::source)
                 ? parse_external_tileset(object, path, *first)
                 : parse_tileset_common(object, path, *first);
    } else {
      return false;
    }
  }

  [[nodiscard]] auto parse_tilesets(const object_type& root,
                                    const QFileInfo& path) -> bool
  {
    if (!m_parser.contains_tilesets(root)) {
      return with_error(parse_error::map_missing_tilesets);
    }

    return std::ranges::all_of(m_parser.tilesets(root),
                               [&](const object_type& ts) {
                                 return parse_tileset(ts, path);
                               });
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
      return with_error(parse_error::could_not_read_external_tileset);
    }
  }

  [[nodiscard]] auto parse_tileset_common(const object_type& object,
                                          const QFileInfo& path,
                                          const tile_id firstGid) -> bool
  {
    const auto tw = object.integer(element_id::tile_width);
    if (!tw) {
      return with_error(parse_error::tileset_missing_tile_width);
    }

    const auto th = object.integer(element_id::tile_height);
    if (!th) {
      return with_error(parse_error::tileset_missing_tile_height);
    }

    const auto relativePath = m_parser.tileset_image_relative_path(object);
    if (!relativePath) {
      return with_error(parse_error::tileset_missing_image_path);
    }

    const auto absolutePath = path.dir().absoluteFilePath(*relativePath);
    if (!QFileInfo{absolutePath}.exists()) {
      return with_error(parse_error::external_tileset_does_not_exist);
    }

    try {
      auto tileset = std::make_shared<core::tileset>(firstGid,
                                                     absolutePath,
                                                     tile_width{*tw},
                                                     tile_height{*th});

      const auto name = object.string(element_id::name);
      if (!name) {
        return with_error(parse_error::tileset_missing_name);
      }

      tileset->set_name(*name);
      tileset->set_path(absolutePath);

      m_document->add_tileset(std::move(tileset));
    } catch (...) {
      return with_error(parse_error::could_not_create_tileset);
    }

    return true;
  }

  [[nodiscard]] auto parse_tile_layer(const object_type& object)
      -> shared<core::layer>
  {
    const auto rows = object.integer(element_id::height);
    if (!rows) {
      m_error = parse_error::layer_missing_height;
      return nullptr;
    }

    const auto cols = object.integer(element_id::width);
    if (!cols) {
      m_error = parse_error::layer_missing_width;
      return nullptr;
    }

    auto layer = std::make_shared<core::tile_layer>(row_t{*rows}, col_t{*cols});

    if (!m_parser.add_tiles(*layer, object, m_error)) {
      return nullptr;
    }

    return layer;
  }

  [[nodiscard]] auto parse_object_layer(const object_type& object)
      -> shared<core::layer>
  {
    auto layer = std::make_shared<core::object_layer>();

    // TODO parse array of objects

    return layer;
  }

  [[nodiscard]] auto parse_layer(const object_type& object, const bool isFirst)
      -> bool
  {
    if (!m_parser.validate_layer_type(object)) {
      return with_error(parse_error::layer_missing_type);
    }

    const auto id = object.integer(element_id::id);
    if (!id) {
      return with_error(parse_error::layer_missing_id);
    }

    shared<core::layer> layer;
    if (m_parser.is_tile_layer(object)) {
      layer = parse_tile_layer(object);

    } else if (m_parser.is_object_layer(object)) {
      layer = parse_object_layer(object);

    } else {
      return with_error(parse_error::unknown_layer_type);
    }

    if (!layer) {
      return false;
    }

    layer->set_visible(object.boolean(element_id::visible).value_or(true));
    layer->set_opacity(object.floating(element_id::opacity, 1.0));
    layer->set_name(object.string(element_id::name, TACTILE_QSTRING(u"Layer")));

    const layer_id layerId{*id};
    m_document->add_layer(layerId, layer);

    if (isFirst) {
      m_document->select_layer(layerId);
    }

    return true;
  }

  [[nodiscard]] auto parse_layers(const object_type& root) -> bool
  {
    if (!m_parser.contains_layers(root)) {
      return with_error(parse_error::map_missing_layers);
    }

    bool first = true;
    for (const auto& layer : m_parser.layers(root)) {
      if (!parse_layer(layer, first)) {
        return false;
      }
      first = false;
    }

    return true;
  }

  [[nodiscard]] auto parse_map_property(const object_type& object) -> bool
  {
    const auto propName = object.string(element_id::name).value();

    // The following is a quirk due to the fact that the type attribute can be
    // omitted for string properties in the XML-format
    QString propType;
    if (m_parser.assume_string_property(object)) {
      propType = TACTILE_QSTRING(u"string");
    } else {
      propType = object.string(element_id::type).value();
    }

    core::property prop;

    if (propType == TACTILE_QSTRING(u"string")) {
      prop.set_value(object.string(element_id::value).value());

    } else if (propType == TACTILE_QSTRING(u"int")) {
      prop.set_value(object.integer(element_id::value).value());

    } else if (propType == TACTILE_QSTRING(u"float")) {
      prop.set_value(object.floating(element_id::value).value());

    } else if (propType == TACTILE_QSTRING(u"bool")) {
      prop.set_value(object.boolean(element_id::value).value());

    } else if (propType == TACTILE_QSTRING(u"file")) {
      const auto file = object.string(element_id::value).value();
      prop.set_value(QFileInfo{file});

    } else if (propType == TACTILE_QSTRING(u"object")) {
      const auto obj = object.integer(element_id::value).value();
      prop.set_value(core::object_ref{obj});

    } else if (propType == TACTILE_QSTRING(u"color")) {
      const auto color = object.string(element_id::value).value();
      prop.set_value(QColor{color});

    } else {
      return with_error(parse_error::unknown_property_type);
    }

    m_document->add_property(propName, prop);

    return true;
  }

  [[nodiscard]] auto parse_map_properties(const object_type& root) -> bool
  {
    return std::ranges::all_of(m_parser.properties(root),
                               [this](const object_type& prop) {
                                 return parse_map_property(prop);
                               });
  }
};

}  // namespace tactile::tmx
