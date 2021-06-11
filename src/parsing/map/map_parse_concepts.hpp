#pragma once

#include <QFileInfo>  // QFileInfo
#include <QString>    // QString
#include <concepts>   // same_as
#include <vector>     // vector

#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tile_layer.hpp"

namespace tactile::parse {

// clang-format off

template <typename T>
concept is_object = requires(T t, ElementId id, const QString& str)
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

template <typename Engine, typename Document, typename Object>
concept is_engine = is_object<Object> &&
                    requires(Engine e,
                             const Document& document,
                             const Object& object,
                             const QFileInfo& path,
                             parse_error& error,
                             row_t nRows,
                             col_t nCols)
{
  { e.from_file(path) } -> std::same_as<maybe<Document>>;
  { e.root(document)  } -> std::same_as<Object>;

  { e.layers(object)                     } -> std::same_as<std::vector<Object>>;
  { e.tilesets(object)                   } -> std::same_as<std::vector<Object>>;
  { e.properties(object)                 } -> std::same_as<std::vector<Object>>;
  { e.objects(object)                    } -> std::same_as<std::vector<Object>>;
  { e.tiles(object, nRows, nCols, error) } -> std::same_as<core::tile_matrix>;

  { e.contains_layers(object)             } -> std::same_as<bool>;
  { e.contains_tilesets(object)           } -> std::same_as<bool>;
  { e.validate_layer_type(object)         } -> std::same_as<bool>;
  { e.tileset_image_relative_path(object) } -> std::same_as<maybe<QString>>;

  { e.is_object_layer(object) } -> std::same_as<bool>;
  { e.is_tile_layer(object)   } -> std::same_as<bool>;
  { e.is_point(object)        } -> std::same_as<bool>;
};

// clang-format on

}  // namespace tactile::parse
