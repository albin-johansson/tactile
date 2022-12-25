# Tactile YAML Format

In addition to the Tiled JSON and XML formats, Tactile supports its own custom YAML map format.
If you're familiar with the Tiled JSON format, you'll notice many similarities.
However, one of the key differences is that the Tactile YAML format doesn't support embedded tilesets.
All identifiers use `kebab-case`.

All attributes that aren't specified as required may be omitted from save files.

## Root

The root node in the Tactile YAML format provides general information about the tilemap.

Required attributes: `version`, `row-count`, `column-count`, `tile-width`, `tile-height`, `next-layer-id`, `next-object-id`.

|               Attribute |       Type       | Description                           |
|------------------------:|:----------------:|:--------------------------------------|
|               `version` |      `int`       | The version of the YAML map format.   |
|             `row-count` |      `int`       | The number of rows in the tilemap.    |
|          `column-count` |      `int`       | The number of columns in the tilemap. |
|            `tile-width` |      `int`       | The width of tiles in the tilemap.    |
|           `tile-height` |      `int`       | The height of tiles in the tilemap.   |
|         `next-layer-id` |      `int`       | The next available layer ID.          |
|        `next-object-id` |      `int`       | The next available object ID.         |
|           `tile-format` |   `TileFormat`   |                                       |
| `component-definitions` | `ComponentDef[]` |                                       |
|                `layers` |    `Layer[]`     |                                       |
|              `tilesets` |  `TilesetRef[]`  |                                       |
|            `properties` |   `Property[]`   |                                       |
|            `components` |  `Component[]`   |                                       |

---

## `TileFormat`

Provides information about the representation of tile layer data.

Using a `encoding` other than `plain` leads to tile layer data being encoded as a one-dimensional array of 32-bit integers.
The tiles are arranged by appending each row after another, starting at the row at index zero (the top row in the editor).
Compressed tile identifiers are always stored using little endian byte ordering.

The emitter works by first compressing the tile data (unless `compression` is set to `none`), and then encoding the possibly compressed data.
Omitting `encoding` or setting it to `plain` leads to tile layer data to be simply stored as a string of integers separated by spaces (potentially formatted with newlines).

Note, it's not possible to specify a compression strategy whilst using `plain` encoding!

The `zlib-compression-level` attribute directly corresponds to Zlib constants such as `Z_DEFAULT_COMPRESSION` (which is `-1`), `Z_BEST_SPEED` (which is `1`), and `Z_BEST_COMPRESSION` (which is `9`).
You can use other values in the interval `[1, 9]` to request an intermediate compression mode, depending on the tradeoff you want to make.

Required attributes: N/A.

|                Attribute | Type     |           Possible values            | Default | Description                      |
|-------------------------:|:---------|:------------------------------------:|:-------:|:---------------------------------|
|               `encoding` | `string` |         `plain`, or `base64`         | `plain` | Tile layer data encoding.        |
|            `compression` | `string` |      `none`, `zlib`, or `zstd`       | `none`  | The compression algorithm used.  |
| `zlib-compression-level` | `int`    | Either `-1` or in the range `[1, 9]` |  `-1`   | Optional Zlib compression level. |
| `zstd-compression-level` | `int`    |        In the range `[1, 19]`        |   `3`   | Optional Zstd compression level. |

Example:

```YAML
encoding: base64
compression: zlib
zlib-compression-level: 9
```

---

## `Layer`

TODO v2: consider removing `id`

Required attributes: `name`, `id`, `type`.

|    Attribute | Type         | Description                                      |
|-------------:|:-------------|:-------------------------------------------------|
|       `name` | `string`     | The name associated with the layer.              |
|         `id` | `int`        | The unique identifier associated with the layer. |
|       `type` | `string`     | `tile-layer`, `object-layer`, or `group-layer`   |
|    `opacity` | `float`      | The opacity of the layer, in the range [0, 1].   |
|    `visible` | `bool`       | Whether or not the layer is rendered.            |
| `properties` | `Property[]` |                                                  |

Examples:

```YAML
name: foo
id: 7
type: tile-layer
data: |
  1 2 3 4
  5 6 7 8
```

```YAML
name: bar
id: 8
type: tile-layer
data: 1 2 3 4 5 6 7 8
```

In addition to the layer attributes listed above, each of the three different kinds of layers have additional attributes.

### `TileLayer`

Required attributes: `data`.

| Attribute |   Type   | Description                                                   |
|----------:|:--------:|:--------------------------------------------------------------|
|    `data` | `string` | A string of (potentially compressed) global tile identifiers. |

### `ObjectLayer`

Required attributes: `objects`.

| Attribute |    Type    |
|----------:|:----------:|
| `objects` | `Object[]` |

### `GroupLayer`

Required attributes: `layers`.

| Attribute |   Type    |
|----------:|:---------:|
|  `layers` | `Layer[]` |

---

## `Object`

Required attributes: `id`, `type`.

|    Attribute |     Type     | Default | Description                   |
|-------------:|:------------:|:-------:|:------------------------------|
|         `id` |    `int`     |   N/A   |                               |
|       `type` |   `string`   |   N/A   | `point`, `rect`, or `ellipse` |
|       `name` |   `string`   |  `""`   |                               |
|        `tag` |   `string`   |  `""`   | A user-defined type tag.      |
|    `visible` |    `bool`    | `true`  |                               |
|          `x` |   `float`    |   `0`   |                               |
|          `y` |   `float`    |   `0`   |                               |
|      `width` |   `float`    |   `0`   |                               |
|     `height` |   `float`    |   `0`   |                               |
| `properties` | `Property[]` |   N/A   |                               |

Example:

```YAML
name: foo
id: 42
type: point
tag: spawn-point
x: 123
y: 456
```

---

## `TilesetRef`

The `TilesetRef` node provides information about an external tileset definition, and are always stored in the main tilemap file.

TODO v2: change `first-global-id` to `first-tile`.

Required attributes: `first-global-id`, `path`.

|         Attribute |   Type   | Description                                           |
|------------------:|:--------:|:------------------------------------------------------|
| `first-global-id` |  `int`   | The first global tile ID associated with the tileset. |
|            `path` | `string` | Relative path to the tileset definition file.         |

Example:

```YAML
first-global-id: 42
path: foo/bar/tileset.yaml
```

---

## `Tileset`

The `Tileset` node is always defined in an external YAML file, since embedded tilesets are not supported by the Tactile YAML format.
By default, a tileset that is added to a map in the Tactile editor will be stored next to the main map file.
Note, the `tiles` node only contains tiles that feature additional data, such as properties and animations.

Required attributes: `version`, `name`, `tile-count`, `tile-width`, `tile-height`, `column-count`, `image-path`, `image-width`, `image-height`.

|      Attribute |     Type     | Description                                |
|---------------:|:------------:|:-------------------------------------------|
|      `version` |    `int`     |                                            |
|         `name` |   `string`   | The name of the tileset.                   |
|   `tile-count` |    `int`     | The total amount of tiles in the tileset.  |
|   `tile-width` |    `int`     | The width of each tile in the tileset.     |
|  `tile-height` |    `int`     | The height of each tile in the tileset.    |
| `column-count` |    `int`     | The amount of tile columns in the tileset. |
|   `image-path` |   `string`   | Relative path to the tileset image.        |
|  `image-width` |    `int`     | The width of the tileset image.            |
| `image-height` |    `int`     | The height of the tileset image.           |
|        `tiles` |   `Tile[]`   |                                            |
|   `properties` | `Property[]` |                                            |

Example:

```YAML
version: 1
name: foo
tile-count: 1024
tile-width: 32
tile-height: 32
column-count: 32
image-path: ../foo.png
image-width: 1024
image-height: 1024
```

### `Tile`

The `Tile` node provides additional information about tiles in a tileset.

TODO v2: change `id` to `index`.

Required attributes: `id`.

|    Attribute |    Value     | Description                                  |
|-------------:|:------------:|:---------------------------------------------|
|         `id` |    `int`     | Index of the tile in the associated tileset. |
|  `animation` |  `Frame[]`   |                                              |
|    `objects` |  `Object[]`  |                                              |
| `properties` | `Property[]` |                                              |

Example:

```YAML
id: 42
animation:
  - tile: 123
    duration: 150
  - tile: 456
    duration: 120
```

### `Frame`

The `Frame` node represents a frame in an animation.

TODO v2: change `tile` to `tile-index`.

Required attributes: `tile`, `duration`.

|  Attribute | Type  | Description                                                    |
|-----------:|:-----:|:---------------------------------------------------------------|
|     `tile` | `int` | Local ID of the tile that should be rendered during the frame. |
| `duration` | `int` | Duration of the frame, in milliseconds.                        |

---

## `ComponentDef`

Required attributes: `name`.

|    Attribute |           Type            | Description                |
|-------------:|:-------------------------:|:---------------------------|
|       `name` |         `string`          | The name of the component. |
| `attributes` | `ComponentDefAttribute[]` |                            |

## `ComponentDefAttribute`

Required attributes: `name`, `type`.

| Attribute |   Type    | Description                                                     |
|----------:|:---------:|:----------------------------------------------------------------|
|    `name` | `string`  | The name of the attribute.                                      |
|    `type` | `string`  | `string`, `int`, `float`, `bool`, `color`,  `file`, or `object` |
| `default` | `variant` | The default attribute value.                                    |

Example:

```YAML
name: GameObject
attributes:
  - name: opacity
    type: float
    default: 1.0
  - name: health
    type: int
    default: 100
  - name: tag
    type: string
```

---

## `Component`

Required attributes: `type`.

| Attribute |          Type          | Description                                |
|----------:|:----------------------:|:-------------------------------------------|
|    `type` |        `string`        | The name of the component definition type. |
|  `values` | `ComponentAttribute[]` |                                            |                                           |

## `ComponentAttribute`

Required attributes: `name`, `value`.

| Attribute |   Type    | Description                    |
|----------:|:---------:|:-------------------------------|
|    `name` | `string`  | The name of the attribute.     |
|   `value` | `variant` | Must be a valid property type. |

Example:

```YAML
type: Physics
values:
  - name: simulated
    value: false
  - name: collision-detection
    value: continuous
```

---

## `Property`

Required attributes: `name`, `type`, `value`.

Note, color properties are always stored using RGBA encoding, with a `#` prefix, e.g. `"#11223344"`.

| Attribute |   Type    | Description                                                    |
|----------:|:---------:|:---------------------------------------------------------------|
|    `name` | `string`  | Name that is unique in the scope of sibling properties.        |
|    `type` | `string`  | `string`, `int`, `float`, `bool`, `color`, `file`, or `object` | 
|   `value` | `variant` |                                                                |

Examples:

```YAML
name: Scale
type: float
value: 0.7
```

```YAML
name: Tint
type: color
value: "#4FEB19FF"
```

```YAML
name: Associated object
type: object
value: 42
```

```YAML
name: Raw data
type: file
value: foo/bar/data.bin
```
