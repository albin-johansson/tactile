#pragma once

#include <QDir>       // QDir
#include <QFileInfo>  // QFileInfo
#include <concepts>   // same_as
#include <cstddef>    // size_t
#include <ranges>     // all_of
#include <vector>     // vector

#include "element_id.hpp"
#include "map_document.hpp"
#include "map_parse_concepts.hpp"
#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "object_id.hpp"
#include "parse_error.hpp"
#include "tactile_qstring.hpp"
#include "to_map_document.hpp"
#include "to_property.hpp"

namespace tactile::parse {

template <typename Engine>
class map_parser final
{
 public:
  using engine_type = Engine;
  using document_type = typename engine_type::document_type;
  using object_type = typename engine_type::object_type;

  static_assert(is_engine<engine_type, document_type, object_type>,
                "The supplied type isn't a parser engine!");

  explicit map_parser(const QFileInfo& path)
  {
    m_data.path = path.absoluteFilePath();
    if (const auto file = open_file(path)) {
      const auto root = m_engine.root(*file);
      if (!parse_next_layer_id(root)) {
        return;
      }

      if (!parse_tilesets(root, path)) {
        return;
      }

      if (!parse_layers(root)) {
        return;
      }

      if (auto properties = parse_properties(root)) {
        m_data.properties = std::move(*properties);
      } else {
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
  engine_type m_engine;
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

    if (auto contents = m_engine.from_file(path)) {
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

    const auto relativePath = m_engine.tileset_image_relative_path(object);
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
        m_engine.from_file(path.dir().absoluteFilePath(*source));
    if (external) {
      return parse_tileset_common(m_engine.root(*external), path, firstGid);
    } else {
      return with_error(parse_error::could_not_read_external_tileset);
    }
  }

  [[nodiscard]] auto parse_tileset(const object_type& object,
                                   const QFileInfo& path) -> bool
  {
    if (const auto gid = parse_tileset_first_gid(object)) {
      const auto hasSource = object.contains(element_id::source);
      return hasSource ? parse_external_tileset(object, path, *gid)
                       : parse_tileset_common(object, path, *gid);
    } else {
      return false;
    }
  }

  [[nodiscard]] auto parse_tilesets(const object_type& root,
                                    const QFileInfo& path) -> bool
  {
    if (!m_engine.contains_tilesets(root)) {
      return with_error(parse_error::map_missing_tilesets);
    }

    return std::ranges::all_of(m_engine.tilesets(root),
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

    tileLayer.tiles =
        m_engine.tiles(object, tileLayer.nRows, tileLayer.nCols, m_error);
    if (m_error != parse_error::none) {
      return false;
    }

    layer.data = std::move(tileLayer);
    return true;
  }

  [[nodiscard]] auto parse_object_layer(layer_data& layer,
                                        const object_type& root) -> bool
  {
    object_layer_data objectLayer;

    const auto emptyString = TACTILE_QSTRING(u"");

    for (const auto& elem : m_engine.objects(root)) {
      auto& object = objectLayer.objects.emplace_back();

      if (const auto id = elem.integer(element_id::id)) {
        object.id = object_id{*id};
      } else {
        return with_error(parse_error::object_missing_id);
      }

      object.x = elem.floating(element_id::x, 0);
      object.y = elem.floating(element_id::y, 0);
      object.width = elem.floating(element_id::width, 0);
      object.height = elem.floating(element_id::height, 0);
      object.name = elem.string(element_id::name, emptyString);
      object.customType = elem.string(element_id::type, emptyString);
      object.visible = elem.boolean(element_id::visible).value_or(true);
      object.isPoint = m_engine.is_point(elem);

      if (auto properties = parse_properties(elem)) {
        object.properties = std::move(*properties);
      } else {
        return false;
      }
    }

    layer.data = std::move(objectLayer);
    return true;
  }

  [[nodiscard]] auto parse_layer(const object_type& object) -> bool
  {
    if (!m_engine.validate_layer_type(object)) {
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

    if (m_engine.is_tile_layer(object)) {
      layer.type = core::layer_type::tile_layer;

      if (!parse_tile_layer(layer, object)) {
        return false;
      }
    } else if (m_engine.is_object_layer(object)) {
      layer.type = core::layer_type::object_layer;

      if (!parse_object_layer(layer, object)) {
        return false;
      }
    } else {
      return with_error(parse_error::unknown_layer_type);
    }

    if (auto properties = parse_properties(object)) {
      layer.properties = std::move(*properties);
    } else {
      return false;
    }

    m_data.layers.emplace_back(std::move(layer));
    return true;
  }

  [[nodiscard]] auto parse_layers(const object_type& root) -> bool
  {
    if (!m_engine.contains_layers(root)) {
      return with_error(parse_error::map_missing_layers);
    }

    return std::ranges::all_of(m_engine.layers(root),
                               [this](const object_type& layer) {
                                 return parse_layer(layer);
                               });
  }

  [[nodiscard]] auto parse_property(const object_type& prop)
      -> maybe<property_data>
  {
    property_data data;
    data.name = prop.string(element_id::name).value();

    const auto type = m_engine.property_type(prop);
    if (auto property = to_property(prop, type, m_error)) {
      data.property = std::move(*property);
      return data;
    } else {
      return std::nullopt;
    }
  }

  [[nodiscard]] auto parse_properties(const object_type& obj)
      -> maybe<std::vector<property_data>>
  {
    const auto props = m_engine.properties(obj);

    std::vector<property_data> result;
    result.reserve(static_cast<std::size_t>(props.size()));

    for (const auto& elem : props) {
      if (auto property = parse_property(elem)) {
        result.emplace_back(*property);
      } else {
        return std::nullopt;
      }
    }

    return result;
  }
};

}  // namespace tactile::parse
