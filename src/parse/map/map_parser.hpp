#pragma once

#include <QDir>       // QDir
#include <QFileInfo>  // QFileInfo
#include <concepts>   // same_as
#include <ranges>     // all_of
#include <vector>     // vector

#include "element_id.hpp"
#include "map_document.hpp"
#include "map_file_type.hpp"
#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "object_id.hpp"
#include "parse_error.hpp"
#include "tactile_qstring.hpp"
#include "to_map_document.hpp"

namespace tactile::parse {

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
                             tile_layer_data& layerData,
                             parse_error& error,
                             const QFileInfo& path)
{
  { parser.root(document)                      } -> std::same_as<Object>;
  { parser.layers(object)                      } -> std::same_as<std::vector<Object>>;
  { parser.tilesets(object)                    } -> std::same_as<std::vector<Object>>;
  { parser.properties(object)                  } -> std::same_as<std::vector<Object>>;
  { parser.from_file(path)                     } -> std::same_as<maybe<Document>>;
  { parser.add_tiles(layerData, object, error) } -> std::same_as<bool>;
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

  static_assert(is_parser<parser_type, document_type, object_type>,
                "The supplied type isn't a parser engine!");

  explicit map_parser(const QFileInfo& path)
  {
    if (const auto file = open_file(path)) {
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

  /**
   * \brief Creates and returns a map document based on the parsed data.
   *
   * \pre There mustn't have been any errors parsing the file.
   *
   * \details You must claim ownership of the returned pointer, preferably by
   * setting the parent of the map document as soon as possible.
   *
   * \return an owning pointer to the associated map document, can be null.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto make_document() -> core::map_document*
  {
    Q_ASSERT(m_error == parse_error::none);
    return to_map_document(m_data);
  }

  /**
   * \brief Indicates whether or not the parser was successful.
   *
   * \return `true` if there were no errors when parsing the file; `false`
   * otherwise.
   *
   * \since 0.1.0
   */
  explicit operator bool() const noexcept
  {
    return m_error == parse_error::none;
  }

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

 private:
  parser_type m_parser;
  parse_error m_error{parse_error::none};
  map_data m_data;

  [[nodiscard]] auto with_error(const parse_error error) noexcept -> bool
  {
    m_error = error;
    return false;
  }

  template <typename T>
  [[nodiscard]] auto with_error(const parse_error error) noexcept -> maybe<T>
  {
    m_error = error;
    return std::nullopt;
  }

  [[nodiscard]] auto open_file(const QFileInfo& path) -> maybe<document_type>
  {
    if (!path.exists()) {
      return with_error<document_type>(parse_error::map_file_not_found);
    }

    if (auto contents = m_parser.from_file(path)) {
      return contents;
    } else {
      return with_error<document_type>(parse_error::could_not_parse_file);
    }
  }

  [[nodiscard]] auto parse_next_layer_id(const object_type& root) -> bool
  {
    if (const auto id = root.integer(element_id::next_layer_id)) {
      m_data.nextLayerId = layer_id{*id};
      return true;
    } else {
      return with_error(parse_error::map_missing_next_layer_id);
    }
  }

  [[nodiscard]] auto parse_tileset_first_gid(const object_type& object)
      -> maybe<tile_id>
  {
    if (const auto gid = object.integer(element_id::first_gid)) {
      return tile_id{*gid};
    } else {
      return with_error<tile_id>(parse_error::tileset_missing_first_gid);
    }
  }

  [[nodiscard]] auto parse_tileset_common(const object_type& object,
                                          const QFileInfo& path,
                                          const tile_id firstGid) -> bool
  {
    tileset_data tileset;
    tileset.firstId = firstGid;

    if (const auto tw = object.integer(element_id::tile_width)) {
      tileset.tileWidth = tile_width{*tw};
    } else {
      return with_error(parse_error::tileset_missing_tile_width);
    }

    if (const auto th = object.integer(element_id::tile_height)) {
      tileset.tileHeight = tile_height{*th};
    } else {
      return with_error(parse_error::tileset_missing_tile_height);
    }

    const auto relativePath = m_parser.tileset_image_relative_path(object);
    if (!relativePath) {
      return with_error(parse_error::tileset_missing_image_path);
    }

    const auto absolutePath = path.dir().absoluteFilePath(*relativePath);
    if (QFileInfo{absolutePath}.exists()) {
      tileset.absolutePath = absolutePath;
    } else {
      return with_error(parse_error::external_tileset_does_not_exist);
    }

    if (const auto name = object.string(element_id::name)) {
      tileset.name = *name;
    } else {
      return with_error(parse_error::tileset_missing_name);
    }

    m_data.tilesets.emplace_back(std::move(tileset));
    return true;
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

  [[nodiscard]] auto parse_tile_layer(layer_data& layer,
                                      const object_type& object) -> bool
  {
    tile_layer_data tileLayer;

    if (const auto rows = object.integer(element_id::height)) {
      tileLayer.nRows = row_t{*rows};
    } else {
      return with_error(parse_error::layer_missing_height);
    }

    if (const auto cols = object.integer(element_id::width)) {
      tileLayer.nCols = col_t{*cols};
    } else {
      return with_error(parse_error::layer_missing_width);
    }

    if (!m_parser.add_tiles(tileLayer, object, m_error)) {
      return false;
    }

    layer.data = std::move(tileLayer);
    return true;
  }

  [[nodiscard]] auto parse_object_layer(layer_data& layer,
                                        const object_type& object) -> bool
  {
    object_layer_data objectLayer;

    // TODO parse array of objects

    layer.data = std::move(objectLayer);
    return true;
  }

  [[nodiscard]] auto parse_layer(const object_type& object, const bool isFirst)
      -> bool
  {
    if (!m_parser.validate_layer_type(object)) {
      return with_error(parse_error::layer_missing_type);
    }

    layer_data layer;

    if (const auto id = object.integer(element_id::id)) {
      layer.id = layer_id{*id};
    } else {
      return with_error(parse_error::layer_missing_id);
    }

    layer.name = object.string(element_id::name, TACTILE_QSTRING(u"Layer"));
    layer.opacity = object.floating(element_id::opacity, 1.0);
    layer.visible = object.boolean(element_id::visible).value_or(true);

    if (m_parser.is_tile_layer(object)) {
      layer.type = core::layer_type::tile_layer;
      if (!parse_tile_layer(layer, object)) {
        return false;
      }

    } else if (m_parser.is_object_layer(object)) {
      layer.type = core::layer_type::object_layer;
      if (!parse_object_layer(layer, object)) {
        return false;
      }

    } else {
      return with_error(parse_error::unknown_layer_type);
    }

    m_data.layers.emplace_back(std::move(layer));
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

  [[nodiscard]] auto to_property(const object_type& object,
                                 const QString& propType)
      -> maybe<core::property>
  {
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
      return with_error<core::property>(parse_error::unknown_property_type);
    }

    return prop;
  }

  [[nodiscard]] auto parse_map_property(const object_type& object) -> bool
  {
    property_data data;
    data.name = object.string(element_id::name).value();

    QString type;
    // The following is a quirk due to the fact that the type attribute can be
    // omitted for string properties in the XML-format
    if (m_parser.assume_string_property(object)) {
      type = TACTILE_QSTRING(u"string");
    } else {
      type = object.string(element_id::type).value();
    }

    if (auto prop = to_property(object, type)) {
      data.property = std::move(*prop);
      m_data.properties.emplace_back(std::move(data));
      return true;
    } else {
      return false;
    }
  }

  [[nodiscard]] auto parse_map_properties(const object_type& root) -> bool
  {
    return std::ranges::all_of(m_parser.properties(root),
                               [this](const object_type& prop) {
                                 return parse_map_property(prop);
                               });
  }
};

}  // namespace tactile::parse
